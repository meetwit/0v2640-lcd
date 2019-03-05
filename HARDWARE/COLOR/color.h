#ifndef __COLOR_H
#define __COLOR_H 
#include "sys.h"   

extern char *rgbNameCN[];
extern char *rgbNameEN[];
extern u32 rgbHex[];

u8 rgb16ToR(u16 rgb16);

u8 rgb16ToG(u16 rgb16);

u8 rgb16ToB(u16 rgb16);

u8 rgb24ToR(u32 rgb24);

u8 rgb24ToG(u32 rgb24);

u8 rgb24ToB(u32 rgb24);

u16 least_squares(u8 numA,u8 numB);

u8 colorIdtif(u16 rgb16);

void show_color(void);
#endif















