#ifndef _Speed_H_
#define _Speed_H_

void Speedpwm_Init(void);                             //速度控制通过pwm 初始化
int SpeedPWM_PID_Out(int SetCount);                   //速度控制通过pwm 输  出  

int Speed_Count(int count, PTX_n ptx_n);              //FTM计数值 正反转I/O 数据有正负
float Speed_Count_Out(void);                          //速度值输出(内部计算用)

int Idle_Judge(void);                                 //空转判断
unsigned int Report_Idle(void);
#endif
