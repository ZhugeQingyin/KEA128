/*********************************************************************************************************************
					模拟IIC接线定义
					------------------------------------ 
						SCL                 F0
						SDA                 F1
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _Peripheral_IIC_h
#define _Peripheral_IIC_h



#include "headfile.h"



#define Peripheral_SCL    F0                           //定义SCL引脚F0
#define Peripheral_SDA    F1                           //定义SDA引脚F1

typedef enum IIC       //DAC模块
{
    IIC,
    SCCB
} IIC_type;



void  IIC_start(void);
void  IIC_stop(void);
void  IIC_ack_main(uint8 ack_main);
void  send_ch(uint8 c);
uint8 read_ch(void);
void  simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat);
uint8 simiic_read_reg(uint8 dev_add, uint8 reg, IIC_type type);
void  IIC_init(void);











#endif

