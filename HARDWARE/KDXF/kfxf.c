#include "kfxf.h"
#include "delay.h"		 
#include "usart2.h"	 	 
#include "string.h"	 	 
#include "color.h"	 


void sendToXfs5152ce(u8 *sendText,u8 lenthText){
	u8 i = 0;
	PcTx_Byte2(0xfd);							//֡ͷ��ע
	PcTx_Byte2(0x00);							//�ı����ȸ��ֽ�
	PcTx_Byte2(lenthText+2);			//�ı����ȵ��ֽ�
	PcTx_Byte2(0x01);							//�ı��ϳ�����
	PcTx_Byte2(0x00);							//�ı�����
	for(i=0;i<lenthText;i++){
		PcTx_Byte2(sendText[i]);
	}
}

void show_sound(u8 index){
	char str1[50];
	if(res_t2 == 0x4F&&heartBeat>3){
		sprintf(str1,"[h0] %s %s",rgbNameCN[index],rgbNameEN[index]);
		sendToXfs5152ce((u8 *)str1,strlen(str1));
		heartBeat = 0;
	}
}






