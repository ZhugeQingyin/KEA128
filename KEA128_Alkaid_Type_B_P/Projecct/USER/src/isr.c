#include "headfile.h"

/*输出与角度限幅*/
#define PWMOut_Max 7000
#define Angle_Max 40.0f
#define Angle_Min 10.0f
#define Idle_Max 1500
/*Send_Mode:蓝牙模式选择 0->实时上传 2->陀螺仪数据上传(虚拟示波器用) 3->低密度上传*/
#define Send_Mode 0

unsigned int Running_time = 0;
float Angle_Increment = 0.0f;

void PIT_CH0_IRQHandler(void)
{
    PIT_FlAG_CLR(pit0); //清除中断标志
    static unsigned char i = 0;
    static int mpu_gyro_x = 0, mpu_gyro_y = 0, mpu_gyro_z = 0; //三轴角加速度
    static int mpu_acc_x = 0, mpu_acc_y = 0, mpu_acc_z = 0;    //三轴加速度
    static float Angle = 0.0f;
    static float Angle_Set = 0.0f;
    static int PWMOut_A = 0;
    static int PWMOut_D = 0;
    static int PWMOut_S = 0;
    static int PWMOut_L = 0;
    static int PWMOut_R = 0;
    static unsigned int Idle = 0;
    i++;
    switch (i)
    {
    case 1:
    {
        Get_Gyro(&mpu_gyro_x, &mpu_gyro_y, &mpu_gyro_z);
        Get_AccData(&mpu_acc_x, &mpu_acc_y, &mpu_acc_z);  
        break;
    }
    case 2:
    {
        Angle_Set = Pass_Parameter(12) + Angle_Increment;
        Angle = Kalman(mpu_gyro_x, mpu_gyro_y, mpu_gyro_z, mpu_acc_x, mpu_acc_y, mpu_acc_z);
        PWMOut_A = Stand(Angle, Angle_Set);
        break;
    }

    case 3:
    {
        PWMOut_S = SpeedPWM_PID_Out(Speed_Ctrl());
        Idle = Report_Idle();
        break;
    }
    case 4:
    {
        PWMOut_D = Basic_Steer(Angle);
        if(Idle == 1)
        {
            PWMOut_D = 0;
        }
        break;
    }
    case 5:
    {
        i = 0;

        Running_time++;
        
        PWMOut_A += PWMOut_S;

        PWMOut_L = PWMOut_A - PWMOut_D;
        PWMOut_R = PWMOut_A + PWMOut_D;
        
        PWMOut_L = fabs(PWMOut_L) > PWMOut_Max ? PWMOut_L > 0 ? PWMOut_Max : -PWMOut_Max : PWMOut_L;
        PWMOut_R = fabs(PWMOut_R) > PWMOut_Max ? PWMOut_R > 0 ? PWMOut_Max : -PWMOut_Max : PWMOut_R;

        if(Idle == 1)
        {
            PWMOut_L = fabs(PWMOut_L) > Idle_Max ? PWMOut_L > 0 ? Idle_Max : -Idle_Max : PWMOut_L;
            PWMOut_R = fabs(PWMOut_R) > Idle_Max ? PWMOut_R > 0 ? Idle_Max : -Idle_Max : PWMOut_R;
        }
        
        if (Angle < Angle_Min || Angle > Angle_Max)
        {
            PWMOut_L = 0;
            PWMOut_R = 0;
        }

        if(Start_flag() == 0)
        {
            PWMOut_L = 0;
            PWMOut_R = 0;
            Running_time = 0;
            
        }
            
        Motor_L(PWMOut_L);
        Motor_R(PWMOut_R);
        Bluetooth(Send_Mode);
        
        Copy_Data(2, PWMOut_L);
        Copy_Data(3, PWMOut_R);
        Copy_Data(20, Running_time);
        break;
    }
    default:
        i = 0;
        break;
    }
}

void KBI0_IRQHandler(void)
{
    volatile unsigned long int flag = 0;
    flag = READ_KBI0_FLAG;
    switch(flag)
    {
    case Stop_C6:
        Stop(1);
        break;
    case Stop_C7:
        Stop(2);
        break;
    default:
        Stop(4);
        break;   
    }
    CLEAN_KBI0_FLAG;
}

void KBI1_IRQHandler(void)
{
    volatile unsigned long int flag = 0;
    flag = READ_KBI1_FLAG;
    switch (flag)
    {
    case K3:
        Key_Menu_Up();
        break;
    case K4:
        Key_Menu_Dn();
        break;
    case K5:
        Key_Value_Up();
        break;
    case K6:
        Key_Value_Dn();
        break;
    default:
        break;
    }
    CLEAN_KBI1_FLAG;
}


void PIT_CH1_IRQHandler(void)
{
    float Angle = Pass_Parameter(12);
    PIT_FlAG_CLR(pit1);      //清除中断标志
    L_Value_Avr();           //电感值调试
    Road_Analysis(Angle);    //道路分析
    Display(2);
}

void RTC_IRQHandler(void)
{
    static unsigned int Seconds = 0;
    /* Clears the interrupt flag, RTIF, and interrupt request */
    RTC_SC |= RTC_SC_RTIF_MASK;
    /* RTC interrupts every 1 Second */
    Seconds++;
}


unsigned int Report_time(void)
{
    return Running_time;
}
    
void Angle_adjustment(float Increment)
{
    Angle_Increment = Increment;
}


    
/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器 通道0得中断
void PIT_CH0_IRQHandler(void)
{
;
}
记得进入中断后清除标志位

FTMRE_IRQHandler      
PMC_IRQHandler        
IRQ_IRQHandler        
I2C0_IRQHandler       
I2C1_IRQHandler       
SPI0_IRQHandler       
SPI1_IRQHandler       
UART0_IRQHandler 
UART1_IRQHandler 
UART2_IRQHandler 
ADC0_IRQHandler       
ACMP0_IRQHandler      
FTM0_IRQHandler       
FTM1_IRQHandler       
FTM2_IRQHandler       
RTC_IRQHandler        
ACMP1_IRQHandler      
PIT_CH0_IRQHandler    
PIT_CH1_IRQHandler    
KBI0_IRQHandler       
KBI1_IRQHandler       
Reserved26_IRQHandler 
ICS_IRQHandler        
WDG_IRQHandler        
PWT_IRQHandler        
MSCAN_Rx_IRQHandler   
MSCAN_Tx_IRQHandler   
*/
