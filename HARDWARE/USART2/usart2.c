#include "sys.h"
#include "usart2.h"	  
u8 res_t2;
//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//串口2初始化 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/5/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV,OVER8设置为0
	mantissa=temp;				 			//得到整数部分
	fraction=(temp-mantissa)*16; 			//得到小数部分,OVER8设置为0	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<0;   					//使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  					//使能串口2时钟 
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
 	GPIO_AF_Set(GPIOA,2,7);					//PA2,AF7
	GPIO_AF_Set(GPIOA,3,7);		 			//PA3,AF7  	   
	//波特率设置
 	USART2->BRR=mantissa; 	// 波特率设置	 
	USART2->CR1|=1<<3;  	//串口发送使能  
	
	USART2->CR1|=1<<2;  		//串口接收使能
	USART2->CR1|=1<<5;    		//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 

	USART2->CR1|=1<<13;  	//串口使能
}

/************************************************************
函数名：USART2_IRQHandler()
形  参：无
返回值：无
功  能：串口2中断
作  者：meetwit
时  间：2017年11月19日11:27:43
*************************************************************/
void USART2_IRQHandler(void)
{
	if(USART2->SR&(1<<5))//接收到数据
	{
	res_t2=USART2->DR;	
	}		
} 

/************************************************************
函数名：PcTx_Byte2(u8 Tx_data)
形  参：u8 Tx_data
返回值：无
功  能：串口2发送一个字节
作  者：meetwit
时  间：2017年11月19日11:26:55
*************************************************************/
void PcTx_Byte2(u8 Tx_data){
	while(!(USART2->SR &(0x1<<7)));
	USART2 ->DR = Tx_data;
}



