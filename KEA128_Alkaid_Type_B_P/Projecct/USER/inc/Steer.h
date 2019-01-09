#ifndef _Steer_H
#define _Steer_H

void Steer_Init(void);             //引导初始化
void L_Value_Avr(void);            //电感值平均
float Road_Analysis(float Angle);  //道路分析
int Basic_Steer(float Angle);      //基础引导
int Speed_Ctrl(void);              //目标速度控制
int Start_flag(void);              //起跑信号
/*差比和修正函数*/
float Fit_F1(float Normalization);
float Fit_F2(float Normalization);
float Fit_F3(float Normalization);
float Fit_F4(float Normalization);
float Fit_F5(float Normalization);
float Fit_F6(float Normalization);
/*模糊自适应PID控制函数类*/
void Fuzzy_Calcu(float Value,float *LeftTemp, float *RightTemp, int *LeftIndex, int *RightIndex);
float Fit_Fuzzy(float Normalization, float *Kp, float *Kd);//Fuzzy adaptive PID control 
#endif


/*关于模糊自适应PID控制*/
/*
    模糊自适应PID控制是在PID算法的基础上，以误差e和误差变化率de作为输入，利用模糊规则进行模糊推理，
查询模糊矩阵表进行参数调整，来满足不同时刻的e和de对PID参数自整定的要求。
    该算法仅对现有控制效果进行增强，即对于该算法，PID参数不能乱给。
*/