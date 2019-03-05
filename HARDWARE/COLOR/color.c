#include "color.h"
#include "delay.h"
#include "lcd.h"
#include "dcmi.h"
#include "usart.h"

#define COLORNUM 55
char *rgbNameCN[]={"红色","绿色","蓝色","粉红","猩红","深粉色","兰花的紫色","紫罗兰","紫红色","深紫罗兰色","深兰花紫","靛青","板岩暗蓝灰色","深岩暗蓝灰色","熏衣草花的淡紫色","午夜的蓝色","深蓝色","皇家蓝","矢车菊的蓝色","淡钢蓝","石板灰","钢蓝","天蓝色","深天蓝","军校蓝","蔚蓝色","青色","深石板灰","深青色","绿宝石","碧绿色","春天的绿色","淡绿色","深绿色","绿黄色","米色","纯黄","橄榄","卡其布","金","小麦色","橙色","桃色","沙棕色","巧克力","珊瑚","橙红色","番茄","淡珊瑚色","棕色","栗色","纯白","灰色","深灰色","纯黑"};
char *rgbNameEN[]={"red","green","blue","pink","crimson","deeppink","orchid","violet","fuchsia","purple","darkvoilet","darkorchid","indigo","slateblue","darkslateblue","lavender","midnightblue","darkblue","royalblue","cornflowerblue","lightsteelblue","slategray","steelblue","skyblue","deepskyblue","cadetblue","azure","cyan","darkslategray","darkcyan","turquoise","auqamarin","springgreen","lightgreen","darkgreen","greenyellow","beige","yellow","olive","khaki","gold","wheat","orange","peachpuff","sandybrown","chocolate","coral","orangered","tomato","lightcoral","brown","maroon","white","gray","darkgray","black"};
u32 rgbHex[]={0xff0000,0x00ff00,0x0000ff,0xffc0cb,0xdc143c,0xff1493,0xda70d6,0xee82ee,0xff00ff,0x800080,0x9400d3,0x9932cc,0x4b0082,0x6a5acd,0x483d8b,0xe6e6fa,0x191970,0x00008b,0x4169e1,0x6495ed,0xb0c4de,0x708090,0x4682b4,0x87ceeb,0x00bfff,0x5f9ea0,0xf0ffff,0x00ffff,0x2f4f4f,0x008b8b,0x40e0d0,0x7fffaa,0x3cb371,0x90ee90,0x006400,0xadff2f,0xf5f5dc,0xffff00,0x808000,0xf0e68c,0xffd700,0xf5deb3,0xffa500,0xffdab9,0xf4a460,0xd2691e,0xff7f50,0xff4500,0xff6347,0xf08080,0xa52a2a,0x800000,0xffffff,0x808080,0xa9a9a9,0x000000};


u8 rgb16ToR(u16 rgb16){
	u8 r;
	r = ((rgb16&0xF800)>>11)*8;
	return r;
}


u8 rgb16ToG(u16 rgb16){
	u8 g;
	g = ((rgb16&0x7E0)>>5)*4;
	return g;
}



u8 rgb16ToB(u16 rgb16){
	u8 b;
	b = (rgb16&0x1F)*8;
	return b;
}


u8 rgb24ToR(u32 rgb24){
	u8 r;
	r = (rgb24&0xff0000)>>16;
	return r;
}



u8 rgb24ToG(u32 rgb24){
	u8 g;
	g = (rgb24&0xff00)>>8;
	return g;
}



u8 rgb24ToB(u32 rgb24){
	u8 b;
	b = rgb24&0xFF;
	return b;
}


u16 least_squares(u8 numA,u8 numB){
	u16 temp = 0;
	u8 a = 0;
	if(numA>numB){
		a = numA - numB;
	}else{
		a = numB -numA;
	}
	temp = a*a;
	return temp;
}


u8 colorIdtif(u16 rgb16){
	u8 i = 0,j = 0;
	u32 temp = 0xffffffff,lastTemp = 0xffffffff;
	for(i=0;i<COLORNUM;i++){
		temp = least_squares(rgb16ToR(rgb16),rgb24ToR(rgbHex[i])) + least_squares(rgb16ToG(rgb16),rgb24ToG(rgbHex[i])) + least_squares(rgb16ToB(rgb16),rgb24ToB(rgbHex[i]));
		if(temp<lastTemp){
			lastTemp = temp;
			j=i;
		}
	}
	return j;
}


void show_color(void){
	u8 i;
	u16 c;
	u8 j;
	DCMI_Stop();
		c = LCD_ReadPoint(120,160);
		j = colorIdtif(c);
		show_sound(j);
		LCD_ShowString(0,0,200,24,24,(u8 *)rgbNameEN[j]); 
		for(i=0;i<12;i++){
			LCD_Fast_DrawPoint(120,155+i,RED);
			LCD_Fast_DrawPoint(115+i,160,RED);		
		}
			
	DCMI_Start(); 		//启动传输
}



















