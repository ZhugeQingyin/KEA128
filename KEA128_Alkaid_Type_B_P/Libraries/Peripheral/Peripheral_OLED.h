/*********************************************************************************************************************	
					接线定义：
					------------------------------------ 
					    OLED液晶    单片机                        
    					SCL         H0
    					SDA         H1            
    					RES         E5    
    					DC          E6
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _Peripheral_OELD_H
#define _Peripheral_OELD_H

#include "headfile.h"



//----宏定义OLED引脚----	 
#define  OLED_SCL_PIN	H0
#define  OLED_SDA_PIN	H1
#define  OLED_RST_PIN	E5
#define  OLED_DC_PIN	E6



void Dly_ms(uint16 ms);
void OLED_Init(void);
void OLED_Fill(uint8 dat);
void OLED_WrDat(uint8 data);
void OLED_Wr6Dat(uint8 data);
void OLED_PutPixel(uint8 x,uint8 y);
void OLED_ClrPixel(uint8 x,uint8 y);
void OLED_Set_Pos(uint8 x, uint8 y);
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_HEXACSII(uint16 hex,uint8* Print);
void OLED_Print_Num(uint8 x, uint8 y, uint16 num);
void OLED_Print_Num1(uint8 x, uint8 y, int16 num);
void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);



/********************************************************************/

#endif
