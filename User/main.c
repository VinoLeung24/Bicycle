/**
  ******************************************************************************
  * @file    main.c
  * @author  vino
  * @version V1.0
  * @date    2017-1-22
  * @brief   整合手套 & LCD板
  ******************************************************************************
  * @attention
  *
  * 平台:STM32F103ZE 

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
			
			case STATE_FREEDOM_MODE:					//自由模式  (主界面) 界面显示 心率值 心电图 速度 体温
			    if(tim3_count == 10)
				{
					get_heart_rate(Signal,BPM,SendBuff);
					get_te(SendBuff);
					reInitDMA4();
					tim3_count = 0;
				}

			break;     
			     
			case STATE_SPORT_MODE_LEVEL:				//自由模式  (平路模式) 界面显示 心率值 心电图 速度 体温
				if(tim3_count == 10)
			    {
			    	get_heart_rate(Signal,BPM,SendBuff);
			    	get_te(SendBuff);
			    	reInitDMA4();
			    	tim3_count = 0;
			    }

			break;
			     	
			case STATE_SPORT_MODE_CLIMB:				//自由模式  (爬坡模式) 界面显示 心率值 心电图 速度 体温
			     if(tim3_count == 10)
			     {
			     	get_heart_rate(Signal,BPM,SendBuff);
			     	get_te(SendBuff);
			     	reInitDMA4();
			     	tim3_count = 0;
			     }

			break;       
			    
			case STATE_SPORT_MODE_WARM1:				//自由模式  (热身模式1)
				
			break;
			
			case STATE_SPORT_MODE_WARM2:				//自由模式  (热身模式2)
				
			break;
		
			case STATE_SPORT_MODE_WARM3:				//自由模式  (热身模式3)
				
			break;
		
			case STATE_USER_REMENBER_DATA:

			break;
			
			case STATE_USER_CHECK:

			break;
		
		}        
	}            
                 
}                
                 
/*********************************************END OF FILE**********************/
                 


