/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usartӦ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  *
  ******************************************************************************
  */
  
#include "usart.h"

uint8_t SendBuff[SENDBUFF_SIZE]={'a','d','d',' ','2',',','0',',','a','d',0xff,0xff,0xff,
								 'h','e','a','r','t','_','r','a','t','e','.','v','a','l','=','b','p','m',0xff,0xff,0xff,
								 't','e','m','p','e','r','.','t','x','t','=','\"','t','e','.','m','p','\"',0xff,0xff,0xff,
								 's','p','e','e','d','.','t','x','t','=','\"','0','0','.','0','0','\"',0xff,0xff,0xff
								}; 

uint8_t SEND_EULER[EULER_SEND_SIZE] =  {'t','1','2','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','1','3','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','1','4','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','1','5','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','1','6','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','1','7','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','1','8','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','1','9','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										't','2','0','.','v','a','l','=','0','x','x','x',0xff,0xff,0xff,
										};

void USART1_DMA_Config(void)
{
		DMA_InitTypeDef DMA_InitStructure;
	
		/*����DMAʱ��*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

		/*����DMAԴ���������ݼĴ�����ַ*/
//		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	  
		DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( USART1->DR ) );	

		/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

		/*���򣺴��ڴ浽����*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

		/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;

		/*�����ַ����*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*�ڴ��ַ����*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*�������ݵ�λ*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*�ڴ����ݵ�λ 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMAģʽ������ѭ��*/
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*���ȼ�����*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*����DMA1��4ͨ��*/		   
		DMA_Init(macUSART_TX_DMA_CHANNEL, &DMA_InitStructure); 	   
		
		/*ʹ��DMA*/
		DMA_Cmd (macUSART_TX_DMA_CHANNEL,ENABLE);					
		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
}

/// ����USART1�����ж�
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


 /**
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* ʹ�ܴ���1�����ж� */
	NVIC_Configuration();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}

/// ����USART3�����ж�
static void NVIC_Configuration_3(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������USARTx_Config
 * ����  ��USART3 GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO), ENABLE);

	/* USART3 GPIO config */
    /* Configure USART3 Tx (PB.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	    
	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure); 
	
	/*	�����ж����ȼ� */
	NVIC_Configuration_3();
	/* ʹ�ܴ���3�����ж� */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
//	USART_ClearFlag(USART3, USART_FLAG_TC);
}

/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  ָ�����ȵķ����ַ��� **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}

void reInitDMA4(void)
{
	DMA1_Channel4->CCR&=~(1<<0);  //�ر�DMA���� 
	DMA1_Channel4->CMAR = (u32)SendBuff;	//Ҫ���������
	DMA1_Channel4->CNDTR=SENDBUFF_SIZE;  //DMA1,����������
	DMA1_Channel4->CCR|=1<<0;  //����DMA����
}

void euler_send(void)
{
	DMA1_Channel4->CCR&=~(1<<0);  			//�ر�DMA���� 
	DMA1_Channel4->CMAR = (u32)SEND_EULER;	//Ҫ���������
	DMA1_Channel4->CNDTR=EULER_SEND_SIZE;  	//DMA1,����������
	DMA1_Channel4->CCR|=1<<0;  				//����DMA����
}

void send_back(uint8_t mode)
{
//	Usart_SendByte(USART3,0xAA);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x03);
//	Usart_SendByte(USART3,0x55);
	Usart_SendByte(USART3,'m');
	Usart_SendByte(USART3,mode);
}

void check_right(void)
{
	printf("click t1,1");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	
	printf("click t10,1");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
}

void shouder_err(void)
{
	printf("click p2,1");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
}

void shouder_ok(void)
{
	printf("click t10,1");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
}

void back_err(void)
{
	printf("click p1,1");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
}

void back_ok(void)
{
	printf("click t1,1");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
}

void click_t0_0(void)
{
	printf("click t0,0");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
}

void click_t0_1(void)
{
	printf("click t7,0");
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
	Usart_SendByte(USART1,0xFF);
}

void request_speed(void)
{
//	Usart_SendByte(USART3,0xAA);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x55);
	Usart_SendByte(USART3,'v');
}

void clear_speed(void)
{
//	Usart_SendByte(USART3,0xAA);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x55);
	Usart_SendByte(USART3,'n');
}

void request_freq(void)
{
//	Usart_SendByte(USART3,0xAA);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x55);
	Usart_SendByte(USART3,'f');
}

void clear_freq(void)
{
//	Usart_SendByte(USART3,0xAA);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x00);
//	Usart_SendByte(USART3,0x55);
	Usart_SendByte(USART3,'d');
}

/*********************************************END OF FILE**********************/
