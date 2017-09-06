#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */

#define all_led_close    	   {led_freq_r_off;			led_myfreq_r_off;   \
								led_freq_l_off;   		led_myfreq_l_off;   \
								guiding_lights_ok_off	guiding_lights_err_off	\
								}


#define led_myfreq_l_on    	   	GPIOF->BSRR |= GPIO_Pin_14				    
#define led_myfreq_l_off       	GPIOF->BRR |= GPIO_Pin_14
	
#define led_myfreq_r_on    	   	GPIOE->BSRR |= GPIO_Pin_8			        
#define led_myfreq_r_off       	GPIOE->BRR |= GPIO_Pin_8
								
#define led_myfreq_off		   	led_myfreq_r_off;	\
								led_myfreq_l_off;
	
#define led_freq_r_on    	   	GPIOE->BSRR |= GPIO_Pin_7	            
#define led_freq_r_off    	   	GPIOE->BRR |= GPIO_Pin_7
	
#define led_freq_l_on    	   	GPIOF->BSRR |= GPIO_Pin_15	            
#define led_freq_l_off    	   	GPIOF->BRR |= GPIO_Pin_15	

#define led_freq_off			led_freq_l_off;	\
								led_freq_r_off;
									
#define led_freq_l_toggle      	GPIOE->ODR ^=GPIO_Pin_7;
#define led_freq_r_toggle      	GPIOF->ODR ^=GPIO_Pin_15;

#define guiding_lights_ok_on   	GPIOG->BSRR |= GPIO_Pin_1;
#define guiding_lights_ok_off   GPIOG->BRR |= GPIO_Pin_1;

#define guiding_lights_err_on  	GPIOG->BSRR |= GPIO_Pin_0;
#define guiding_lights_err_off  GPIOG->BRR |= GPIO_Pin_0;

extern void LED_GPIO_Config(void);
void ZigbeeGpioInit(void);

#endif /* __LED_H */
