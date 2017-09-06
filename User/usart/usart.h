#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>

#define SENDBUFF_SIZE     75
#define EULER_SEND_SIZE  135

/**************************USART参数定义********************************/
#define             macUSART_BAUD_RATE                       115200

#define             macUSART_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             macUSART_CLK                             RCC_APB2Periph_USART1
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             macUSART_TX_PORT                         GPIOA   
#define             macUSART_TX_PIN                          GPIO_Pin_9
#define             macUSART_RX_PORT                         GPIOA 
#define             macUSART_RX_PIN                          GPIO_Pin_10
#define             macUSART_IRQ                             USART1_IRQn
#define             macUSART_INT_FUN                         USART1_IRQHandler

#define             macUSART_TX_DMA_CHANNEL                  DMA1_Channel4



void USART1_Config( void );
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void USART1_DMA_Config(void);
void reInitDMA4(void);
void euler_send(void);
void USART3_Config(void);
void send_back(uint8_t mode);
void send_lock(uint8_t mode);
void check_right(void);
void shouder_err(void);
void shouder_ok(void);
void back_err(void);
void back_ok(void);
void click_t0_0(void);
void click_t0_1(void);
void request_speed(void);
void clear_speed(void);
void request_freq(void);
void clear_freq(void);
#endif /* __USART1_H */
