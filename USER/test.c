#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "usart2.h"  
#include "timer.h" 
#include "ov5640.h" 
#include "dcmi.h" 
#include "color.h" 
#include "kfxf.h"
#include "string.h"

u8 ovx_mode=0;							//bit0:0,RGB565ģʽ;1,JPEGģʽ 

#define jpeg_buf_size   31*1024		//����JPEG���ݻ���jpeg_buf�Ĵ�С(*4�ֽ�)
__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG���ݻ���buf
volatile u32 jpeg_data_len=0; 			//buf�е�JPEG��Ч���ݳ��� 
volatile u8 jpeg_data_ok=0;				//JPEG���ݲɼ���ɱ�־ 
										//0,����û�вɼ���;
										//1,���ݲɼ�����,���ǻ�û����;
										//2,�����Ѿ����������,���Կ�ʼ��һ֡����
		 
//JPEG�ߴ�֧���б�
const u16 jpeg_img_size_tbl[][2]=
{
	160,120,	//QQVGA 
	320,240,	//QVGA  
	640,480,	//VGA
	800,600,	//SVGA
	1024,768,	//XGA
	1280,800,	//WXGA 
	1440,900,	//WXGA+
	1280,1024,	//SXGA
	1600,1200,	//UXGA	
	1920,1080,	//1080P
	2048,1536,	//QXGA
	2592,1944,	//500W 
};

const u8*EFFECTS_TBL[7]={"Normal","Cool","Warm","B&W","Yellowish ","Inverse","Greenish"};	//7����Ч 
const u8*JPEG_SIZE_TBL[12]={"QQVGA","QVGA","VGA","SVGA","XGA","WXGA","WXGA+","SXGA","UXGA","1080P","QXGA","500W"};//JPEGͼƬ 12�ֳߴ� 

//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{
	if(ovx_mode&0X01)	//ֻ����JPEG��ʽ��,����Ҫ������.
	{
		if(jpeg_data_ok==0)	//jpeg���ݻ�δ�ɼ���?
		{
			DMA2_Stream1->CR&=~(1<<0);		//ֹͣ��ǰ����
			while(DMA2_Stream1->CR&0X01);	//�ȴ�DMA2_Stream1������   
			jpeg_data_len=jpeg_buf_size-DMA2_Stream1->NDTR;//�õ��˴����ݴ���ĳ���
			jpeg_data_ok=1; 				//���JPEG���ݲɼ��갴��,�ȴ�������������
		}
		if(jpeg_data_ok==2)	//��һ�ε�jpeg�����Ѿ���������
		{
			DMA2_Stream1->NDTR=jpeg_buf_size;	//���䳤��Ϊjpeg_buf_size*4�ֽ�
			DMA2_Stream1->CR|=1<<0;				//���´���
			jpeg_data_ok=0;						//�������δ�ɼ�
		}
	}else
	{
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();		//��ʼд��GRAM
	}	
} 

//RGB565����
//RGB����ֱ����ʾ��LCD����
void rgb565_test(void)
{ 
	LCD_Clear(WHITE);
  POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"ALIENTEK STM32F4");
	LCD_ShowString(30,70,200,16,16,"OV5640 RGB565 Mode");
	
	LCD_ShowString(30,100,200,16,16,"KEY0:Contrast");			//�Աȶ�
	LCD_ShowString(30,130,200,16,16,"KEY1:Saturation"); 	//ɫ�ʱ��Ͷ�
	LCD_ShowString(30,150,200,16,16,"KEY2:Effects"); 			//��Ч 
	LCD_ShowString(30,170,200,16,16,"KEY_UP:FullSize/Scale");	//1:1�ߴ�(��ʾ��ʵ�ߴ�)/ȫ�ߴ�����
	
	//�Զ��Խ���ʼ��
	OV5640_RGB565_Mode();	//RGB565ģʽ 
	OV5640_Focus_Init();
	
	OV5640_Light_Mode(0);	//�Զ�ģʽ
	OV5640_Color_Saturation(3);//ɫ�ʱ��Ͷ�0
	OV5640_Brightness(4);	//����0
	OV5640_Contrast(3);		//�Աȶ�0
	OV5640_Sharpness(33);	//�Զ����
	OV5640_Focus_Constant();//���������Խ�
	DCMI_Init();			//DCMI����
	/////////////////////////////////////////////////////////////////////
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,1,1,0);//DCMI DMA����  
 	OV5640_OutSize_Set(4,0,lcddev.width,lcddev.height); 
	sendToXfs5152ce((u8 *)"[h0]��ã�hello",strlen("[h0]��ã�hello"));
	DCMI_Start(); 		//��������	 
	while(1){
		show_color();
		delay_ms(10);	
	}
	 
} 
int main(void)
{        
	Stm32_Clock_Init(336,8,2,7);//����ʱ��,168Mhz 
	delay_init(168);			//��ʱ��ʼ��  
	uart_init(84,115200);		//��ʼ�����ڲ�����Ϊ115200 
	usart2_init(42,115200);		//��ʼ������2������Ϊ921600
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��  
	TIM3_Int_Init(10000-1,8400-1);//10Khz����,1�����ж�һ��
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"OV5640 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/4/30");  	 
	while(OV5640_Init())//��ʼ��OV2640
	{
		LCD_ShowString(30,130,240,16,16,"OV5640 ERROR");
		delay_ms(200);
	  LCD_Fill(30,130,239,170,WHITE);
		delay_ms(200);
		LED0=!LED0;
	} 
	LCD_ShowString(30,130,200,16,16,"OV5640 OK");  
 	rgb565_test(); 
	return 0;
}
















