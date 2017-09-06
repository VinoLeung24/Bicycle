#ifndef __TIME_H__
#define __TIME_H__

#include "bsp_adc.h"

#define true 1
#define false 0
	
#define KEY_ON	0
#define KEY_OFF	1

void SYSTICK_INIT(void);
void TIM_INIT(void);
float GET_NOWTIME(void);
void get_ms(unsigned long *time);

void TIM3_Int_Init(u16 arr,u16 psc);

#endif

