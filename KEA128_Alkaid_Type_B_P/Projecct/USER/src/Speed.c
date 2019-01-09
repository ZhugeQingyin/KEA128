#include "headfile.h"

/*跳轮判断值*/
#define Idle_Min (SetCount + 100)
/*输出限幅*/
#define Output_Max 3000
/*跳轮时的输出限幅*/
#define Output_Idle_Max 50

unsigned int Idle_IN = 0;

struct PID
{
    float SetValue;
    float ActualValue;
    float Err;
    float LastErr;
    float P, I, D;
    double integral;
    double integral_max;
    double Output;
    double LastOutput;
} SpeedPWM_PID;

void Speedpwm_Init(void)
{
    ftm_count_init(ftm0);
    ftm_count_init(ftm1);
    gpio_init(F5, GPI, 0);
    gpio_init(F4, GPI, 0);
    SpeedPWM_PID.SetValue = 0.0f;
    SpeedPWM_PID.ActualValue = 0.0f;
    SpeedPWM_PID.Err = 0.0f;
    SpeedPWM_PID.LastErr = 0.0f;
    SpeedPWM_PID.LastOutput = 0.0f;

    SpeedPWM_PID.P = Pass_Parameter(3);
    SpeedPWM_PID.I = Pass_Parameter(4);
    SpeedPWM_PID.D = 0.0f;

    SpeedPWM_PID.integral = 0.0f;
    SpeedPWM_PID.integral_max = 512 * 30;
}

int SpeedPWM_PID_Out(int SetCount)
{
    float Count = 0;
    int KI = 1;
    Count = Speed_Count_Out();
    if(fabs(Count) > Idle_Min)
    {
        Count = Count > 0 ? Idle_Min : -Idle_Min;
        Idle_IN = 1;
        /*跳轮判断*/
        KI = 0;
    }
    else
    {
        Idle_IN = 0;
    }
    
    SpeedPWM_PID.ActualValue = Count;
    SpeedPWM_PID.SetValue = SetCount;
    SpeedPWM_PID.Err = SpeedPWM_PID.SetValue - SpeedPWM_PID.ActualValue;
    
    /*误差低通滤波*/
    SpeedPWM_PID.LastErr *= 0.7f;
    SpeedPWM_PID.Err *= 0.3f;
    SpeedPWM_PID.Err += KI * SpeedPWM_PID.LastErr;
    SpeedPWM_PID.LastErr = SpeedPWM_PID.Err;
    
    SpeedPWM_PID.integral += SpeedPWM_PID.Err;

    if (fabs(SpeedPWM_PID.integral) > SpeedPWM_PID.integral_max)
    {
        if (SpeedPWM_PID.integral > 0)
        {
            SpeedPWM_PID.integral = SpeedPWM_PID.integral_max;
        }
        else if (SpeedPWM_PID.integral < 0)
        {
            SpeedPWM_PID.integral = -SpeedPWM_PID.integral_max;
        }
    }

    SpeedPWM_PID.Output = SpeedPWM_PID.P * SpeedPWM_PID.Err + SpeedPWM_PID.I * SpeedPWM_PID.integral + SpeedPWM_PID.D * (SpeedPWM_PID.Err - SpeedPWM_PID.LastErr);
    
    /*输出平滑*/
    SpeedPWM_PID.LastOutput *= 0.5f;
    SpeedPWM_PID.Output *= 0.5f;
    SpeedPWM_PID.Output += SpeedPWM_PID.LastOutput;
    
    if(Idle_IN == 1)
    {
        SpeedPWM_PID.Output = fabs(SpeedPWM_PID.Output) > Output_Idle_Max ? SpeedPWM_PID.Output > 0 ? Output_Idle_Max : -Output_Idle_Max : SpeedPWM_PID.Output;
    }
    else
    {
        SpeedPWM_PID.Output = fabs(SpeedPWM_PID.Output) > Output_Max ? SpeedPWM_PID.Output > 0 ? Output_Max : -Output_Max : SpeedPWM_PID.Output;
    }
    
    SpeedPWM_PID.LastOutput = SpeedPWM_PID.Output;
    return (int)(SpeedPWM_PID.Output);
}

int Speed_Count_L(int count)
{
    char ForB = 0;
    ForB = gpio_get(F4);
    ForB = ForB ? 1 : -1;
    count *= ForB;
    return count;
}

int Speed_Count_R(int count)
{
    char ForB = 0;
    ForB = gpio_get(F5);
    ForB = ForB ? -1 : 1;
    count *= ForB;
    return count;
}

float Speed_Count_Out(void)
{
    float Count = 0.0f;
    int Count_L = 0;
    int Count_R = 0;
    Count_L = ftm_count_get(ftm0);
    Count_L = Speed_Count_L(Count_L);
    Count_R = ftm_count_get(ftm1);
    Count_R = Speed_Count_R(Count_R);
    Count = (Count_L + Count_R) / 2.0f;
    ftm_count_clean(ftm0);
    ftm_count_clean(ftm1);
    Copy_Data(1, (int)Count);
    return Count;
}

/*跳轮检测*/
int Idle_Judge(void)
{
    static int Count_Last = 20;
    int Count = 0;
    int Count_L = 0;
    int Count_R = 0;
    Count_L = ftm_count_get(ftm0);
    Count_L = Speed_Count_L(Count_L);
    Count_R = ftm_count_get(ftm1);
    Count_R = Speed_Count_R(Count_R);
    Count = (Count_L + Count_R);
    if(Count > 20)
    {
       if(Count > 2 * Count_Last)
        {
            return 1;
        }
        else
        {
            Count_Last = Count;
            return 0;
        } 
    }
    else
    {
        return 0;
    }
  
}

unsigned int Report_Idle(void)
{
    return Idle_IN;
}

