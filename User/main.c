/**
  ******************************************************************************
  * @file    main.c
  * @author  vino
  * @version V1.0
  * @date    2017-1-22
  * @brief   �������� & LCD��
  ******************************************************************************
  * @attention
  *
  * ƽ̨:STM32F103ZE 

  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bicycle_led.h"
#include "time.h"
#include "delay.h"  
#include "bsp_adc.h" 
#include "usart.h"
#include "bicycle.h"
#include "bicycle_mlx9016.h"

extern int Signal;
extern int BPM;
int Signal_show;
extern uint8_t tim3_count;

BICYCLE mBicycle;

extern uint8_t SendBuff[SENDBUFF_SIZE];
extern uint8_t myFreq_Flag;

uint8_t temp[4];

int main(void)
{	
	LED_GPIO_Config();
	ZigbeeGpioInit();
	mlx_GPIO_Config();
	ADC2_Init();
	TIM3_Int_Init(1999,71);
	delay_init();
	USART1_DMA_Config();
	USART1_Config();
	USART3_Config();
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	 
	while(1)
	{
		switch(mBicycle.outer_status)
		{
			case STATE_START_MODE: 

			break;
			
			case STATE_FREEDOM_MODE:					//����ģʽ  (������) ������ʾ ����ֵ �ĵ�ͼ �ٶ� ����
			    if(tim3_count == 10)
				{
					get_heart_rate(Signal,BPM,SendBuff);
					get_te(SendBuff);
					reInitDMA4();
					tim3_count = 0;
				}

			break;     
			     
			case STATE_SPORT_MODE_LEVEL:				//����ģʽ  (ƽ·ģʽ) ������ʾ ����ֵ �ĵ�ͼ �ٶ� ����
				if(tim3_count == 10)
			    {
			    	get_heart_rate(Signal,BPM,SendBuff);
			    	get_te(SendBuff);
			    	reInitDMA4();
			    	tim3_count = 0;
			    }

			break;
			     	
			case STATE_SPORT_MODE_CLIMB:				//����ģʽ  (����ģʽ) ������ʾ ����ֵ �ĵ�ͼ �ٶ� ����
			     if(tim3_count == 10)
			     {
			     	get_heart_rate(Signal,BPM,SendBuff);
			     	get_te(SendBuff);
			     	reInitDMA4();
			     	tim3_count = 0;
			     }

			break;       
			    
			case STATE_SPORT_MODE_WARM1:				//����ģʽ  (����ģʽ1)
				
			break;
			
			case STATE_SPORT_MODE_WARM2:				//����ģʽ  (����ģʽ2)
				
			break;
		
			case STATE_SPORT_MODE_WARM3:				//����ģʽ  (����ģʽ3)
				
			break;
		
			case STATE_USER_REMENBER_DATA:

			break;
			
			case STATE_USER_CHECK:

			break;
		
		}        
	}            
                 
}                
                 
/*********************************************END OF FILE**********************/
                 


