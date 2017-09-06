/**
  ******************************************************************************
  * @file    bicycle_led.c
  * @author  vino
  * @version V1.0
  * @date    2017-1-22
  * @brief   ����LCD��ledָʾ��
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  
#include "bicycle_led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		

		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF |RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE, ENABLE); 
														   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOF, &GPIO_InitStructure);	
														   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOG, &GPIO_InitStructure);	
														   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOE, &GPIO_InitStructure);	
		
		all_led_close;
}

void ZigbeeGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOG, &GPIO_InitStructure);
//	
	GPIOG->BRR  |= 0X400;								//M0.0 = 0
	GPIOG->BSRR |= 0X800;								//M0.1 = 1
}
/*********************************************END OF FILE**********************/
