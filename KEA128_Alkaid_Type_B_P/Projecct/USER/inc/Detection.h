#ifndef _Detection_H_
#define _Detection_H_

#define LL 0
#define LR 1
#define RL 2
#define RR 3

unsigned int MidAvr(unsigned int value[], int size);//中位平均值
int Detection_init(int SE);                         //初始化  ADC_SE0->ADC_SE通道的ADC
unsigned int Detection_Once(int SE);                //采集一次ADC_SE0->ADC_SE通道的12bit ADC值
int Detection_MidAvr(int SE);                       //采集12次ADC_SE0->ADC_SE通道的ADC值并进行中位值滤波

#endif

/*
IO    通道
PTA0  SE0
PTA1  SE1
PTA6  SE2
PTA7  SE3
PTB0  SE4
PTB1  SE5
PTB2  SE6
PTB3  SE7
----------
PTC0  SE8
PTC1  SE9
PTC2  SE10
PTC3  SE11
PTF4  SE12
PTF5  SE13
PTF6  SE14
PTF7  SE15
*/