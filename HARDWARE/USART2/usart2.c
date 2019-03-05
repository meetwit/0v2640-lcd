#include "sys.h"
#include "usart2.h"	  
u8 res_t2;
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����2��ʼ�� 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV,OVER8����Ϊ0
	mantissa=temp;				 			//�õ���������
	fraction=(temp-mantissa)*16; 			//�õ�С������,OVER8����Ϊ0	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<0;   					//ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  					//ʹ�ܴ���2ʱ�� 
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
 	GPIO_AF_Set(GPIOA,2,7);					//PA2,AF7
	GPIO_AF_Set(GPIOA,3,7);		 			//PA3,AF7  	   
	//����������
 	USART2->BRR=mantissa; 	// ����������	 
	USART2->CR1|=1<<3;  	//���ڷ���ʹ��  
	
	USART2->CR1|=1<<2;  		//���ڽ���ʹ��
	USART2->CR1|=1<<5;    		//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//��2��������ȼ� 

	USART2->CR1|=1<<13;  	//����ʹ��
}

/************************************************************
��������USART2_IRQHandler()
��  �Σ���
����ֵ����
��  �ܣ�����2�ж�
��  �ߣ�meetwit
ʱ  �䣺2017��11��19��11:27:43
*************************************************************/
void USART2_IRQHandler(void)
{
	if(USART2->SR&(1<<5))//���յ�����
	{
	res_t2=USART2->DR;	
	}		
} 

/************************************************************
��������PcTx_Byte2(u8 Tx_data)
��  �Σ�u8 Tx_data
����ֵ����
��  �ܣ�����2����һ���ֽ�
��  �ߣ�meetwit
ʱ  �䣺2017��11��19��11:26:55
*************************************************************/
void PcTx_Byte2(u8 Tx_data){
	while(!(USART2->SR &(0x1<<7)));
	USART2 ->DR = Tx_data;
}



