/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bicycle.h"
#include "bicycle_led.h"
#include "usart.h"
#include "delay.h"

extern uint8_t SEND_EULER[EULER_SEND_SIZE];
extern uint8_t SendBuff[SENDBUFF_SIZE];

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

extern BICYCLE mBicycle;
extern uint8_t tim3_count;
extern uint8_t freq;
extern uint16_t Freq_Count;
extern uint8_t myFreq_Flag;

void USART1_IRQHandler(void)
{
	static uint8_t model_flag ,freq_flag ;
	uint8_t data_rx;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data_rx = USART_ReceiveData(USART1);

		if(model_flag)
		{
			model_flag = 0;
			switch(data_rx)
			{
				case '0':													//返回主界面 初始化  关闭led指示灯  关闭tim3 (心率)
					all_led_close;																
					mBicycle.outer_status = STATE_START_MODE;
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE); 
					tim3_count = 0;											//计时清零
					send_back('0');
					delay_ms(20);
					myFreq_Flag = 0;
				break;
				
				case '1':													//自由模式 打开tim3(心率)	//计时清零									
					all_led_close;	
					mBicycle.outer_status = STATE_FREEDOM_MODE;
					tim3_count = 0;											//计时清零
					send_back('1');
					delay_ms(20);
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
					Freq_Count = 0;
					led_freq_l_on;
					led_freq_r_off;
				break;
				
				case '2':													//自由模式  (平路模式) 打开tim3(心率)
				    all_led_close;	
				    mBicycle.outer_status = STATE_SPORT_MODE_LEVEL;
					tim3_count = 0;											//计时清零
					send_back('2');
					delay_ms(20);
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
					Freq_Count = 0;
					led_freq_l_on;
					led_freq_r_off;
				break;
				
				case '3':													//自由模式 (爬坡模式) 打开tim3(心率)
					all_led_close;				
					mBicycle.outer_status = STATE_SPORT_MODE_CLIMB;
					tim3_count = 0;											//计时清零
					send_back('3');
					delay_ms(20);
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
					Freq_Count = 0;
					led_freq_l_on;
					led_freq_r_off;
				break;
				
				case '4':													//自由模式 (热身模式) 关闭tim3(心率)
					all_led_close;	
					mBicycle.outer_status = STATE_SPORT_MODE_WARM1;
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE); 			
					tim3_count = 0;											//计时清零
					send_back('4');
					delay_ms(20);
					myFreq_Flag = 0;
				break;	
				
				case '5':													//自由模式 (热身模式) 关闭tim3(心率)
					all_led_close;	
					mBicycle.outer_status = STATE_SPORT_MODE_WARM2;
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);
					send_back('5');	
					delay_ms(20);
					tim3_count = 0;											//计时清零
					myFreq_Flag = 0;
				break;
				
				case '6':													//自由模式 (热身模式) 关闭tim3(心率)
					all_led_close;	
					mBicycle.outer_status = STATE_SPORT_MODE_WARM3;
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);
					send_back('6');
					delay_ms(20);
					tim3_count = 0;											//计时清零		
					myFreq_Flag = 0;
				break;
				
				case '7':
                    all_led_close;
                    mBicycle.outer_status = STATE_USER_REMENBER_DATA;
					send_back('7');
					delay_ms(20);
					myFreq_Flag = 0;
				break;
				
				case '8':													//动作扩展  请求背心发送 pitch roll yaw
					all_led_close;
					mBicycle.outer_status = STATE_USER_CHECK;
					send_back('8');
					delay_ms(20);
					myFreq_Flag = 0;
				break;
				
				case '9':													//动作扩展  请求背心发送 pitch roll yaw
					all_led_close;
					mBicycle.outer_status = STATE_CALIBRATION;
					send_back('9');
					delay_ms(20);
					myFreq_Flag = 0;
				break;

				default:
				break;
			}
		}
		else if(freq_flag)
		{
			freq_flag = 0;
			Freq_Count = 0;
			freq = data_rx - '0';
			led_freq_l_on;
			led_freq_r_off;
//			return;
		}
		else
		{
			switch(data_rx)
			{
				case 'm':
					model_flag = 1;
				break;
				case 'f':
					freq_flag = 1;
				break;

				default:
				break;
			}
		}
	}
}

void USART3_IRQHandler(void)
{
	uint8_t data_rx;
 
	static uint8_t slave_flag,slave,mode_flag,data_count,speed_count;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		data_rx = USART_ReceiveData(USART3);

		if(mode_flag == CMD)
		{
			if(slave == BACK)
			{
				switch(data_rx)
				{
					case STATE_DATA_RIGHT:
						check_right();	
						guiding_lights_err_off;
					break;
					
					case STATE_SHOUDER_ERR:
						shouder_err();
						guiding_lights_err_on;
					break;
					
					case STATE_SHOUDER_OK:
						shouder_ok();
//						guiding_lights_err_off;
					break;
					
					case STATE_BACK_ERR:
						back_err();
						guiding_lights_err_on;
					break;
					
					case STATE_BACK_OK:
						back_ok();
//						guiding_lights_err_off;
					break;
					
					case STATE_CLIMB_ERR:
						shouder_err();
						back_err();
						guiding_lights_err_on;
					break;
					
					case STATE_WARM1_OK:
						click_t0_0();
						guiding_lights_err_off;
					break;
					
					case STATE_WARM1_ERR:
						click_t0_1();
						guiding_lights_err_off;
					break;
					
					case STATE_WARM2_RIGHT:
						click_t0_0();
					break;
					
					case STATE_WARM2_LEFT:
						click_t0_0();
					break;
					
					case STATE_WARM3_OK:
						click_t0_0();
					break;
					
					default:
						
					break;
				}
			}
			
			if(slave == SPEED)
			{
				if((mBicycle.outer_status == STATE_FREEDOM_MODE) || (mBicycle.outer_status == STATE_SPORT_MODE_LEVEL) || (mBicycle.outer_status == STATE_SPORT_MODE_LEVEL))
				{
					if(data_rx == 'l')
					{
						led_myfreq_l_on;
						myFreq_Flag = 1;
//						Freq_Count = 0;
					}
					
					if(data_rx == 'r')
					{
						led_myfreq_r_on;
						myFreq_Flag = 1;
//						Freq_Count = 0;
					}
				}
			}
			
			mode_flag = NONE;
			slave = 0;
		}
		
		if(mode_flag == DATA)
		{
			if(slave == BACK)
			{
				if((data_rx & 0x80) == 0)
					SEND_EULER[8+15*data_count] = '0';
				if((data_rx & 0x80) == 0x80)
				{
					data_rx &= 0x7F;
					
					SEND_EULER[8+15*data_count] = '-';
				}
				data_rx *= 2;
				
				SEND_EULER[9+15*data_count] = data_rx/100 + '0';
				SEND_EULER[10+15*data_count] = (data_rx%100)/10 + '0';
				SEND_EULER[11+15*data_count] = data_rx%10 + '0';
				
				data_count++;
				
				if(data_count == 9)
				{
					euler_send();
					data_count = 0;
					mode_flag = NONE;
					slave = 0;
				}	
			}
			
			if(slave == SPEED)
			{
				if(speed_count == 1)
				{
					SendBuff[69] = data_rx/10 + '0';
					SendBuff[70] = data_rx%10 + '0';
					speed_count = 0;
					mode_flag = NONE;
					slave = 0;
					return; 
				}
				
				if(speed_count == 0)
				{
					SendBuff[66] = data_rx/10 + '0';
					SendBuff[67] = data_rx%10 + '0';
					speed_count++;
				}
				
			}
		}
		
		if(slave_flag == 2)
		{
			slave_flag = 0;
			if(data_rx == CMD)
				mode_flag = CMD;
			else if(data_rx == DATA)
				mode_flag = DATA;
			else
				mode_flag = 0;
		}
		
		if(slave_flag == 1)
		{
			slave_flag++;
			if(data_rx == '1')
				slave = SPEED;
			else if(data_rx == '2')
				slave = BACK;
			else
			{
				slave_flag = 0;
				slave = 0;
			}
		}
		
		if(data_rx == 's')
			slave_flag = 1;
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
