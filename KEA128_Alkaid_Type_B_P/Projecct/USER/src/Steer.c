#include "headfile.h"
#include "MathTable.h"

/*横竖电感上下限 L_Value_Avr*/
#define TR_Max 100.0f
#define TL_Max 100.0f
#define TR_Min 0.0f
#define TL_Min 0.0f
#define V_Max 20.0f
/*-------------------------------------------------------------------------------------------*/
/*横竖电感加权 Ring_Judge*/
#define TW 0.2f //Transverse Weighting
#define VW 0.8f // Vertical  Weighting

/*KS_Ring_After  KS_Ring_Before 圆环速度系数*/
#define KS_Ring_A 1.0f
#define KS_Ring_B 1.0f
/*-------------------------------------------------------------------------------------------*/
/*KV_Ring_Before  KV_Ring_Now KV_Ring_After KV_Ring_Out  圆环电压系数*/
#define KV_Ring_B 1.8f
#define KV_Ring_N 2.2f
#define KV_Ring_A 1.8f
#define KV_Ring_O 1.2f

/*出环处理*/
#define Ring_fitting_Max_P1 0.6f
#define Ring_fitting_Max_P2 0.2f

/*Fit拟合系数*/
#define Kfit4 0.065625f
#define Kfit 5.0f

/*赛道电压检测*/
#define Count_Clean 25
/*误操作清除周期*/
#define Ring_Count_Clean 500

/*转向环输出限幅*/
#define Steer_PWMOut_Max 4000.0f

/*--------Fuzzy adaptive PID control---------*/
/*基本模糊集*/
#define NB -5.0f
#define NM -2.5f
#define NS -1.25f
#define ZO 0.0f
#define PS 1.25f
#define PM 2.5f
#define PB 5.0f
/*PID调整系数*/
#define Kp_M 50.0f //Kp_Multiplier
#define Kd_M 50.0f //Kd_Multiplier
/*模糊规则*/
/*推理决策*/
/*Kp*/
static const float fuzzyRuleKp[7][7] = {
    PB, PB, PM, NM, NM, NS, ZO,
    PB, PB, PM, NS, NS, ZO, ZO,
    PM, PM, PS, PS, ZO, PS, PM,
    PM, PS, PS, ZO, PS, PM, PM,
    PS, PS, ZO, PS, PS, PM, PM,
    ZO, ZO, NS, NS, PM, PM, PB,
    ZO, NS, NM, NM, PM, PB, PB};
/*Kd*/
static const float fuzzyRuleKd[7][7] = {
    NB, NM, NS, PB, PB, PB, PB,
    NM, NM, PS, PM, PM, PM, PM,
    NS, PS, PM, PS, PS, PS, PM,
    ZO, ZO, ZO, ZO, ZO, ZO, ZO,
    PM, PS, PS, PS, PM, PS, NS,
    PM, PM, PM, PM, NS, NM, NM,
    PB, PB, PB, PB, NS, NM, NB};
/*模糊规则得到的控制调节值*/
float Kp = 0;
float Kd = 0;
/*-----------------END---------------*/

struct PID
{
    float SetValue;
    float ActualValue;
    float Err;
    float LastErr;
    float P, I, D;
    float integral;
    double Output;
    double Output_last;
} Steer_PID;

/*AD采集值*/
static float LL_f = 0.0f;
static float LR_f = 0.0f;
static float RL_f = 0.0f;
static float RR_f = 0.0f;

/*其他变量*/
float Angle_flash = 0;
int Speed_flash = 0;
int Speed_Reference = 0;

/*Road voltage level*/
static float RVL[5] = {200.0f, 200.0f, 200.0f, 200.0f, 200.0f};

/*允许起跑标志位*/
int Allow_start = 0;

void Steer_Init(void)
{
    Steer_PID.SetValue = 0.0f;
    Steer_PID.ActualValue = 0.0f;
    Steer_PID.Err = 0.0f;
    Steer_PID.LastErr = 0.0f;
    //-------------------------
    Steer_PID.P = 0.0f;
    Steer_PID.I = 0.0f;
    Steer_PID.D = 0.0f;
    //-------------------------
    Steer_PID.integral = 0.0f;
    Steer_PID.Output_last = 0.0f;
    Angle_flash = Pass_Parameter(12);
    Speed_Reference = (int)Pass_Parameter(11);
    Speed_flash = Speed_Reference;
}

void L_Value_Avr(void)
{
    unsigned int LL_value = 0;
    unsigned int LR_value = 0;
    unsigned int RL_value = 0;
    unsigned int RR_value = 0;

    for (int i = 0; i < 12; i++)
    {
        LL_value += Detection_Once(LL);
        LR_value += Detection_Once(LR);
        RL_value += Detection_Once(RL);
        RR_value += Detection_Once(RR);
    }

    LL_f = ((LL_value / 12.0f) / 4095.0f) * 200.0f;
    LR_f = ((LR_value / 12.0f) / 4095.0f) * 200.0f;
    RL_f = ((RL_value / 12.0f) / 4095.0f) * 200.0f;
    RR_f = ((RR_value / 12.0f) / 4095.0f) * 200.0f;

    /*归一化*/
    LL_f = ((LL_f - TL_Min) / TL_Max) * 100.0f;
    LR_f = (LR_f / V_Max) * 100.0f;
    RL_f = (RL_f / V_Max) * 100.0f;
    RR_f = ((RR_f - TR_Min) / TR_Max) * 100.0f;
    /*归一化限幅*/
    LL_f = LL_f > 100.0f ? 100.0f : LL_f;
    LR_f = LR_f > 100.0f ? 100.0f : LR_f;
    RL_f = RL_f > 100.0f ? 100.0f : RL_f;
    RR_f = RR_f > 100.0f ? 100.0f : RR_f;

    Copy_Data(4, (int)LL_f);
    Copy_Data(5, (int)LR_f);
    Copy_Data(6, (int)RL_f);
    Copy_Data(7, (int)RR_f);
}

float Road_Analysis(float Angle)
{
    float Normalization = 0.0f;
    float Normalization_V = 0.0f;
    float Angle_Temp = 0.0f;
    static float Normalization_last = 0.0f;
    static int Ring_flag = 0;
    static int Ring_Out = 0;
    static unsigned int Count = 0;
    static unsigned int Ring_Count = 0;
    static unsigned int LoS = 0; /*Loss of signal*/
    int Road_Analysis_Num = 0;
    float fitting = 0.0f;
    unsigned int Posture_flag = 0;
    Count++;
    
    /*车身姿态检测*/
    Angle_Temp = Angle - Angle_flash;
    if (Angle_Temp <= 12.5f && Angle_Temp >= 0.0f)
    {
        Posture_flag = 1;
    }
    
    /*差比和 及 自适应进圆环*/
    if (LL_f < 1 && RR_f < 1)
    {
        Normalization = Normalization_last;
        LoS++;
        if (LoS >= 50 && Normalization != 0)
        {
            Stop(3);
        }
    }
    else
    {
        Normalization = (LL_f - RR_f) / (LL_f + RR_f);
        LoS = 0;
        Allow_start = 1;
        if (Normalization <= 0.2f && Count >= Count_Clean && Ring_flag == 0 && Ring_Out == 0 && Posture_flag == 1)
        {
            RVL[0] = RVL[1];
            RVL[1] = RVL[2];
            RVL[2] = RVL[3];
            RVL[3] = (LL_f + RR_f) < 40.0f ? 40.0f : (LL_f + RR_f);
            RVL[4] = RVL[0];
            for (int i = 0; i < 4; i++)
            {
                if (RVL[i] < RVL[4])
                {
                    RVL[4] = RVL[i];
                }
            }
            Copy_Data(15, (int)RVL[4]);
        }
        if (Count >= Count_Clean)
        {
            Count = 0;
        }
    }
    
    //-----------------------------Basic_Judge----------------------
    fitting = Fit_F6(Normalization) * Kfit;
    //fitting = Fit_F4(Normalization) * Kfit4;
    //fitting = Fit_Fuzzy(Normalization, &Kp, &Kd) * Kfit;
    
    //-----------------------------Ring_Judge-----------------------
    if (Ring_flag != 0 || Ring_Out != 0 || (LL_f + RR_f) >= (RVL[4] * KV_Ring_N))
    {
        if (Ring_Out == 0)
        {
            Normalization_V = ((TW * LL_f + VW * LR_f) - (VW * RL_f + TW * RR_f)) / ((TW * LL_f + VW * LR_f) + (VW * RL_f + TW * RR_f));
            fitting = Fit_F6(Normalization_V) * Kfit;
            Ring_flag++;
            Road_Analysis_Num = 2;
            Speed_Reference = (int)(KS_Ring_B * Speed_flash);

            if ((LL_f + RR_f) <= (RVL[4] * KV_Ring_B))
            {
                fitting = Fit_F6(Normalization) * Kfit;
                Road_Analysis_Num = 0;
                if(Ring_flag < 3)
                {
                    Ring_Out = 0;
                }
                else
                {
                    Ring_Out = 1;
                }
                Ring_flag = 0;
            }
        }
        else
        {
            Speed_Reference = (int)(KS_Ring_A * Speed_flash);

            if ((RVL[4] * KV_Ring_A) <= (LL_f + RR_f))
            {
                Ring_Out = 2;
            }

            if (((LL_f + RR_f) <= (RVL[4] * KV_Ring_O)) && Ring_Out == 2)
            {
                Ring_Out = 0;
                Ring_Count = 0;
                Steer_PID.LastErr = fitting;
                Speed_Reference = Speed_flash;
            }
        }
    }
    /*误操作清除*/
    if(Ring_flag != 0 || Ring_Out != 0)
    {
        Ring_Count++;
        if(Ring_Count > Ring_Count_Clean)
        {
            Ring_flag = 0;
            Ring_Out = 0;
            Ring_Count = 0;
        }
    }

    //-----------------------------End_All_Judge------------------------ 
    switch (Road_Analysis_Num)
    {
    case 0:
        Steer_PID.P = Pass_Parameter(5) + Kp;
        Steer_PID.I = 0.0f;
        Steer_PID.D = Pass_Parameter(6) + Kd;
        break;
    case 1:
        Steer_PID.P = 0.0f;
        Steer_PID.I = 0.0f;
        Steer_PID.D = 0.0f;
        break;
    case 2:
        Steer_PID.P = Pass_Parameter(7);
        Steer_PID.I = 0.0f;
        Steer_PID.D = Pass_Parameter(8);
        break;
    case 3:
        Steer_PID.P = 0.0f;
        Steer_PID.I = 0.0f;
        Steer_PID.D = 0.0f;
        break;
    }

    Normalization_last = Normalization;

    Copy_Data(9, (int)(Normalization_V * 100));
    Copy_Data(10, (int)(Normalization * 100));
    Copy_Data(11, (int)(fitting * 100));
    Copy_Data(18, Ring_flag);
    Copy_Data(19, Ring_Out);

    return fitting;
}

int Basic_Steer(float Angle)
{
    L_Value_Avr();
    Steer_PID.ActualValue = Road_Analysis(Angle);
    Steer_PID.Err = Steer_PID.SetValue - Steer_PID.ActualValue;
    Steer_PID.integral += Steer_PID.Err;
    Steer_PID.Output = Steer_PID.P * Steer_PID.Err + Steer_PID.I * Steer_PID.integral + Steer_PID.D * (Steer_PID.Err - Steer_PID.LastErr);
    Steer_PID.Output = fabs(Steer_PID.Output) > Steer_PWMOut_Max ? Steer_PID.Output > 0 ? Steer_PWMOut_Max : -Steer_PWMOut_Max : Steer_PID.Output;
    Steer_PID.LastErr = Steer_PID.Err;
    Copy_Data(12, (int)Steer_PID.Output);
    return (int)(Steer_PID.Output);
}

int Speed_Ctrl(void)
{
    return Speed_Reference;
}

int Start_flag(void)
{
    return Allow_start;
}

/*以下F1->F6是转向拟合方案*/
float Fit_F1(float Normalization)
{
    float steer_flag = 0.0f;
    float fitting = 0.0f;
    steer_flag = Normalization > 0 ? 1.0f : -1.0f;
    Normalization = fabs(Normalization);
    if (Normalization < 0.3f)
    {
        fitting = Normalization * Normalization;
        fitting *= 44.4f;
    }
    else
    {
        fitting = log235(Normalization);
    }
    return steer_flag * fitting;
}

float Fit_F2(float Normalization)
{
    float steer_flag = 0.0f;
    float fitting = 0.0f;
    steer_flag = Normalization > 0 ? 1.0f : -1.0f;
    Normalization = fabs(Normalization);
    if (Normalization < 0.3f)
    {
        fitting = Normalization * Normalization * Normalization;
        fitting *= 148.1f;
    }
    else
    {
        fitting = log235(Normalization);
    }
    return steer_flag * fitting;
}

float Fit_F3(float Normalization)
{
    float steer_flag = 0.0f;
    float fitting = 0.0f;
    steer_flag = Normalization > 0 ? 1.0f : -1.0f;
    Normalization = fabs(Normalization);
    fitting = log235(Normalization);
    return steer_flag * fitting;
}

float Fit_F4(float Normalization)
{
    float steer_flag = 0.0f;
    float fitting = 0.0f;
    steer_flag = Normalization > 0 ? 1.0f : -1.0f;
    Normalization = fabs(Normalization);
    fitting = arctan(Normalization);
    fitting = steer_flag * fitting;
    return fitting;
}

float Fit_F5(float Normalization)
{
    float steer_flag = 0.0f;
    float fitting = 0.0f;
    steer_flag = Normalization > 0 ? 1.0f : -1.0f;
    Normalization = fabs(Normalization);
    fitting = powatan(Normalization);
    return steer_flag * fitting;
}

float Fit_F6(float Normalization)
{
    float steer_flag = 0.0f;
    float fitting = 0.0f;
    steer_flag = Normalization > 0 ? 1.0f : -1.0f;
    Normalization = fabs(Normalization);
    fitting = Self(Normalization);
    fitting = steer_flag * fitting;
    return fitting;
}
/*---------------------END-----------------------*/

/*模糊自适应PID算法测试代码(规则参数未整定)*/
/*求隶属度*/
/*隶属度范围*/
/*-5 -2.5 -1.25 0 1.25 2.5 5*/
void Fuzzy_Calcu(float Value, float *LeftTemp, float *RightTemp, int *LeftIndex, int *RightIndex)
{
    if (Value >= 0)
    {
        if (Value < 1.25)
        {
            *LeftIndex = 3;
            *RightIndex = 4;
            *RightTemp = 0.8f * Value;
            *LeftTemp = 1 - *RightTemp;
        }
        else if (Value < 2.5)
        {
            *LeftIndex = 4;
            *RightIndex = 5;
            *RightTemp = 0.8f * (Value - 1.25f);
            *LeftTemp = 1 - *RightTemp;
        }
        else if (Value < 5.0)
        {
            *LeftIndex = 5;
            *RightIndex = 6;
            *RightTemp = 0.4f * (Value - 2.5f);
            *LeftTemp = 1 - *RightTemp;
        }
        else
        {
            *LeftIndex = 5;
            *RightIndex = 6;
            *LeftTemp = 0;
            *RightTemp = 1;
        }
    }
    else
    {
        if (Value > -1.25)
        {
            *LeftIndex = 2;
            *RightIndex = 3;
            *RightTemp = 0.8f * (Value + 1.25f);
            *LeftTemp = 1 - *RightTemp;
        }
        else if (Value > -2.5)
        {
            *LeftIndex = 1;
            *RightIndex = 2;
            *RightTemp = 0.8f * (Value + 2.5f);
            *LeftTemp = 1 - *RightTemp;
        }
        else if (Value > -5.0)
        {
            *LeftIndex = 0;
            *RightIndex = 1;
            *RightTemp = 0.4f * (Value + 5.0f);
            *LeftTemp = 1 - *RightTemp;
        }
        else
        {
            *LeftIndex = 0;
            *RightIndex = 1;
            *LeftTemp = 1;
            *RightTemp = 0;
        }
    }
}
/*模糊控制主调函数*/
float Fit_Fuzzy(float Normalization, float *Kp, float *Kd) //Fuzzy adaptive PID control
{
    float steer_flag = 0.0f;
    float fitting = 0.0f;
    static float fitting_Last = 0.0f;
    /*误差与误差的微分*/
    float e = 0.0f;
    float de = 0.0f;
    /*隶属度*/
    float eLeftTemp, deLeftTemp;
    float eRightTemp, deRightTemp;

    /*模糊表位置*/
    int eLeftIndex, deLeftIndex;
    int eRightIndex, deRightIndex;

    /*差比和处理*/
    steer_flag = Normalization > 0 ? 1.0f : -1.0f;
    Normalization = fabs(Normalization);
    fitting = Self(Normalization);
    fitting = steer_flag * fitting;

    /*模糊自适应PID 开始计算Kp Kd系数变化量*/
    e = fitting;
    de = fitting - fitting_Last;

    /*求隶属*/
    Fuzzy_Calcu(e, &eLeftTemp, &eRightTemp, &eLeftIndex, &eRightIndex);
    Fuzzy_Calcu(de * 1.67f, &deLeftTemp, &deRightTemp, &deLeftIndex, &deRightIndex);

    /*反模糊*/
    *Kp = (eLeftTemp * deLeftTemp * fuzzyRuleKp[deLeftIndex][eLeftIndex] 
           + eLeftTemp * deRightTemp * fuzzyRuleKp[deRightIndex][eLeftIndex] 
               + eRightTemp * deLeftTemp * fuzzyRuleKp[deLeftIndex][eRightIndex] 
                   + eRightTemp * deRightTemp * fuzzyRuleKp[deRightIndex][eRightIndex]);

    *Kd = (eLeftTemp * deLeftTemp * fuzzyRuleKd[deLeftIndex][eLeftIndex] 
           + eLeftTemp * deRightTemp * fuzzyRuleKd[deRightIndex][eLeftIndex] 
               + eRightTemp * deLeftTemp * fuzzyRuleKd[deLeftIndex][eRightIndex] 
                   + eRightTemp * deRightTemp * fuzzyRuleKd[deRightIndex][eRightIndex]);

    /*Kp Kd系数变化量输出幅值调整*/
    *Kp *= Kp_M;
    *Kd *= Kd_M;

    fitting_Last = fitting;
    return fitting;
}
