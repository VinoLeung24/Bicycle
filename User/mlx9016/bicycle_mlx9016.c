#include "bicycle_mlx9016.h"  
#include "usart.h"
unsigned char  DataL,DataH, Pecreg;
extern uint8_t USART_RX_4[9];
void mlx_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOB和GPIOF的外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		

}
void delay(unsigned int x)
{
	while(x!=0)
	{
		x--;
		__NOP();
	}
}


void start_bit(void)
{
    SDA_OUT;
    SDA_H;
    delay(20);
    SCL_H;
    delay(20);
    SDA_L;
    delay(20);
    SCL_L;
    delay(20);
}
void STOP_bit(void)
{       
    SDA_OUT;
	SCL_L;				
	delay(20);
	SDA_L;				
	delay(20);
	SCL_H;			
	delay(20);
	SDA_H;
}

void send_bit(unsigned char bit_out)
{       
  SDA_OUT;
	if(bit_out) 
          SDA_H;	  
	else	 	   
          SDA_L;

	delay(8);
	SCL_H;				
	delay(20);
	SCL_L;					
	delay(20);
	SDA_H;
	
}
unsigned char Receive_bit(void)
{
	unsigned char Ack_bit;
	
	SDA_IN;						
  delay(20);
	SCL_H;					
	delay(20);
	if(SDA_IN_H)	
          Ack_bit=1;			
	else		
          Ack_bit=0;			
	SCL_L;					
	delay(20);
	return	Ack_bit;
}

void tx_byte(unsigned char dat_byte) 
{ 
    unsigned char i,n,dat,rec; 
    n=10; 
    TX_again: 
    dat=dat_byte; 
    for(i=0;i<8;i++) 
    { 
        if(dat&0x80) 
            send_bit(1);
        else 
            send_bit(0);
    dat=dat<<1; 
    } 
    rec=Receive_bit();
    if(rec==1) 
    { 
        STOP_bit(); 
        if(n!=0) 
            {n--;goto Repeat;} 
        else 
            goto exit; 
    } 
    else 
        goto exit; 
    Repeat: 
        start_bit(); 
    goto TX_again; 
    exit: ; 
} 

unsigned char RX_byte(unsigned char ack_nack)
{
	unsigned char 	RX_buffer;
	unsigned char	Bit_Counter;
	
	for(Bit_Counter=8; Bit_Counter; Bit_Counter--)
	{
		if(Receive_bit())						
		{
			RX_buffer <<= 1;					
			RX_buffer |=0x01;
		}
		else
		{
			RX_buffer <<= 1;					
			RX_buffer &=0xfe;	
		}

	}
	send_bit(ack_nack);							
	return RX_buffer;
}

unsigned int memread(void) 
{ 
    start_bit(); 
    tx_byte(0x00);
    tx_byte(0x07); 
    start_bit(); 
    tx_byte(0x01); 
    DataL=RX_byte(0); 
    DataH=RX_byte(0);
    Pecreg=RX_byte(1); 
    STOP_bit(); 
    return(DataH*256+DataL); 
} 

void get_te(uint8_t *te)
{
	uint16_t i;
	i=memread()*2-27315;
	te[46]=i/1000+'0';
	te[47]=i/100%10+'0';
	te[49]=i/10%10+'0';
	te[50]=i%10+'0';
}

void get_heart_rate(int Signal, int BPM, uint8_t *send)
{
	int Signal_show;
	
	if((Signal >> 2) > 100)
	{
		Signal_show = (Signal >> 2) - 100;
	}
	else
	{
		Signal_show = Signal >> 2;
	}
	
	send[8] = (Signal_show / 10) + '0';
	send[9] = (Signal_show % 10) + '0';
	
	send[28] = (BPM / 100) + '0';
	send[29] = ((BPM % 100) / 10) + '0';
	send[30] = (BPM % 10) + '0';
}



/*********************************************END OF FILE**********************/
