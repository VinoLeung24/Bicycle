#include "time.h"
#include "usart.h"
#include "delay.h"                    
#include "bicycle_led.h"
#include "bicycle.h"

// these variables are volatile because they are used during the interrupt service routine!
int BPM;                   			 // used to hold the pulse rate
int Signal;               			 // holds the incoming raw data
int IBI = 600;            			 // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
unsigned char QS = false;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0; // used to determine pulse timing
unsigned long lastBeatTime = 0;  // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
unsigned char firstBeat = true;  // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;// used to seed rate array so we startup with reaso

uint8_t tim3_count = 0;
uint8_t myFreq_Count = 0;
uint8_t myFreq_Flag = 0;
uint16_t Freq_Count = 0;
uint8_t freq = 0;

extern uint8_t SendBuff[SENDBUFF_SIZE];

void SYSTICK_INIT(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */

	if (SysTick_Config(SystemCoreClock/1000))	// ST3.5.0��汾SystemCoreClock/10���ܳ���16777216
	{ 
		/* Capture error */ 
		while (1);
	}
		// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

void TIM_INIT(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 0xffff; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_Cmd(TIM4, ENABLE);
}

float GET_NOWTIME(void)//���ص�ǰsystick������ֵ,32λ
{
	float temp=0 ;
	static uint32_t now=0; // �������ڼ��� ��λ us

 	now = TIM4->CNT;//����16λʱ��
   	TIM4->CNT=0;
	temp = (float)now / 2000000.0f;          //�����ms���ٳ���2�ó��������ڵ�һ��

	return temp;
}

void get_ms(unsigned long *time)
{
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
	 
	TIM_TimeBaseStructure.TIM_Period = arr;      
	TIM_TimeBaseStructure.TIM_Prescaler =psc;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
	/*ѡ��update event��ΪTRGO,����TIM3����ADCͨ�� */
	//ÿ����ʱ���ڽ����󴥷�һ��
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);                 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE); 
	/*�ȹرյȴ�ʹ��*/ 
}

void TIM3_IRQHandler(void)
{
	uint16_t runningTotal=0;
	uint8_t i;
	uint16_t Num;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
			
		Freq_Count++;
		if(Freq_Count == (10 - freq)*50)
		{
			Freq_Count = 0;
			led_freq_l_toggle;
			led_freq_r_toggle;
		}
		
		if(myFreq_Flag)
		{
			myFreq_Count++;
			if(myFreq_Count == 2)
			{
				led_myfreq_off;
				myFreq_Flag = 0;
			}
		}
		
		//��ȡ����ֵ����2λ��12λ-->10λ
		Signal = (ADC_GetConversionValue(ADC2) >> 2);     		// read the Pulse Senso
		
		tim3_count++;

		sampleCounter += 2;                          			// keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;          			// monitor the time since the last beat to avoid noise	
		
		//�������岨�Ĳ���Ͳ���
		//  find the peak and trough of the pulse wave
		if(Signal < thresh && Num > (IBI/5)*3)					// avoid dichrotic noise by waiting 3/5 of last IBI
		{   
			if (Signal < T)										// T is the trough
			{                        			
				T = Signal;                           			// keep track of lowest point in pulse wave 
			}
		}

		if(Signal > thresh && Signal > P)
		{        												// thresh condition helps avoid noise
			P = Signal;                             			// P is the peak
		}                                         				// keep track of highest point in pulse wave

	//��ʼѰ������
	//���������ٵ�ʱ��signal��ֵ������
	//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
	// signal surges up in value every time there is a pulse
	if (Num > 250 )
	{                                 							// avoid high frequency noise
		if ( (Signal > (thresh)) && (Pulse == false) && (Num > (IBI/5)*3) /* && ((P - T) > 100)*/)
		{    
			Pulse = true; 										// set the Pulse flag when we think there is a pulse                        
			IBI = sampleCounter - lastBeatTime;         		// measure time between beats in mS
			lastBeatTime = sampleCounter;               		// keep track of time for next pulse
		
			if(secondBeat)
			{                        							// if this is the second beat, if secondBeat == TRUE
				secondBeat = false;                  			// clear secondBeat flag
				for(i=0; i<=9; i++)
				{                 								// seed the running total to get a realisitic BPM at startup
					rate[i] = IBI;                     				//���ڶ���beat ����rate���� ���˲�ʹ��
				}
			}
		
			if(firstBeat)
			{                         							// if it's the first time we found a beat, if firstBeat == TRUE
				firstBeat = false;                   			// clear firstBeat flag
				secondBeat = true;                   			// set the second beat flag
				return;                              			// ������һ��beat
			}   
		
			// keep a running total of the last 10 IBI values
			// runningTotal = 0;                  		// clear the runningTotal variable    
		
			for(i=0; i<=8; i++)
			{                    								// shift data in the rate array
				rate[i] = rate[i+1];                  			// and drop the oldest IBI value 
				runningTotal += rate[i];              			// add up the 9 oldest IBI values
			}
		
			rate[9] = IBI;                          			// add the latest IBI to the rate array
			runningTotal += rate[9];                			// add the latest IBI to runningTotal
			runningTotal /= 10;                     			// average the last 10 IBI values 
			BPM = 60000/runningTotal;               			// how many beats can fit into a minute? that's BPM!
			QS = true;                              			// set Quantified Self flag 
			// QS FLAG IS NOT CLEARED INSIDE THIS ISR
		}                       
	}

	//���忪ʼ�½�
	if (Signal < thresh && Pulse == true && (Num > (IBI/5)*3) && ((P - T) > 100))
	{   														// when the values are going down, the beat is over
		Pulse = false;                         					// reset the Pulse flag so we can do it again
		amp = P - T;                           					// get amplitude of the pulse wave
		thresh = (amp/2) + T;                    				// set thresh at 50% of the amplitude
		P = thresh;                            					// reset these for next time
		T = thresh;
	}

	//û�м�⵽���壬����Ĭ��ֵ
	if (Num > 1800 /*|| (Signal > 3*(thresh/2))*/)
	{                         									// if 2.5 seconds go by without a beat
		thresh = 512;                          					// set thresh default
		P = 512;                               					// set P default
		T = 512;                               					// set T default
		lastBeatTime = sampleCounter;          					// bring the lastBeatTime up to date 
		firstBeat = true;                      					// set these to avoid noise
		secondBeat = false;                    					// when we get the heartbeat back			
		BPM = 0;
	}

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}


