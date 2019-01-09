#ifndef _Motor_H_
#define _Motor_H_

void Motor_Init();       //电机PWM初始化 C0 C1 C2 C3
void Motor_L(int value); //左电机PWM
void Motor_R(int value); //右电机PWM
void Motor_Stop(void);   //左右电机停车
#endif