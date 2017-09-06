#ifndef __bicycle_mlx9016_H
#define	__bicycle_mlx9016_H
#include "stm32f10x.h"
#define SDA_IN  GPIO_InitTypeDef GPIO_InitStructure;\
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  \
								GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; \
								GPIO_Init(GPIOC, &GPIO_InitStructure)
#define SDA_OUT GPIO_InitTypeDef GPIO_InitStructure;\
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  \
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; \
								GPIO_Init(GPIOC, &GPIO_InitStructure)

#define SDA_H GPIO_SetBits(GPIOC,GPIO_Pin_14)
#define SDA_L GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define SDA_IN_H  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==1
#define SCL_H GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define SCL_L GPIO_ResetBits(GPIOC,GPIO_Pin_13)
extern void mlx_GPIO_Config(void);
extern unsigned int memread(void);
void get_te(uint8_t *te);
void get_heart_rate(int Signal, int BPM, uint8_t *send);
void send_txt(uint8_t data[],uint8_t len);
void add_point(uint8_t * temp);

#endif
