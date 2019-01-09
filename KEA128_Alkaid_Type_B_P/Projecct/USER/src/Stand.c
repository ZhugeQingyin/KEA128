#include "headfile.h"
struct PID
{
    float SetValue;    //设定值
    float ActualValue; //实际值
    float Err;         //误差
    float LastErr;     //上次误差
    float P, I, D;
    float integral;    //积分
    double Output;
} Stand_PID;

void Stand_PID_init()
{

    Stand_PID.SetValue = 0.0f;
    Stand_PID.ActualValue = 0.0f;
    Stand_PID.Err = 0.0f;
    Stand_PID.LastErr = 0.0f;

    Stand_PID.P = Pass_Parameter(1);
    Stand_PID.I = 0.0f;
    Stand_PID.D = Pass_Parameter(2);

    Stand_PID.integral = 0.0f;
}

int Stand(float Angle, float Set)
{
    Stand_PID.ActualValue = Angle;
    Stand_PID.SetValue = Set;
    Stand_PID.Err = Stand_PID.SetValue - Stand_PID.ActualValue;
    Stand_PID.integral += Stand_PID.Err;
    Stand_PID.Output = Stand_PID.P * Stand_PID.Err + Stand_PID.I * Stand_PID.integral + Stand_PID.D * (Stand_PID.Err - Stand_PID.LastErr);
    Stand_PID.LastErr = Stand_PID.Err;
    return (int)Stand_PID.Output;
}
