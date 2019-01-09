/*Project:十三夜之月
**Board  :若你不在身旁,能上苍穹又怎样
**Version:Alkaid 2018-7-10
*/

#include "headfile.h"

void User_Pit_Init(unsigned char PITN,unsigned long int ms);
void User_ADC_Init(void);
void Start_Mode(void);
unsigned char Debug = 0;
unsigned char Start_Mode_Value = 0;
unsigned char Start_speed_up = 1;

int main(void)
{
    get_clk();                 //获取时钟频率 必须执行
    FLASH_Init();              //Flash
    Transfer_Data_Init();      //运行参数传递
    OLED_Init();               //OLED
    IIC_init();                //软件模拟IIC
    InitMPU6050();             //MPU6050
    User_ADC_Init();           //ADC
    uart_init(UART_1, 57600);  //UART
    Steer_Init();              //引导
    Motor_Init();              //PWM
    Speedpwm_Init();           //速度
    Stand_PID_init();          //直立
    Run_Data_Init();           //运行数据采集初始化
    Start_Mode();              //选择启动模式
    EnableInterrupts;          //开启全局中断
    //----------------------------------------------------------------
    while (Debug)
    {
        Key_Dis();
    }
    while (Report_time() <= 600)
    {
        /*1.延迟到6S后开启停车中断*/
        
        /*2.启动加速 6秒或加速到150*/
        if(Read_Data(1) <= 150 && Start_speed_up == 1)
        {
            Angle_adjustment(2.5f);
        }
        else
        {
            Angle_adjustment(0.0f);
            Start_speed_up = 0;
        }
    }
    Angle_adjustment(0.0f);
    Stop_init();
    while (1)
    {
        ;
    }
}

void User_Pit_Init(unsigned char PITN,unsigned long int ms) //定时中断初始化
{
    if(PITN == 0)
    {
        pit_init_ms(pit0, ms);               //初始化pit0 周期设置为 X ms
        set_irq_priority(PIT_CH0_IRQn, 3);   //设置pit0优先级 0-3
        enable_irq(PIT_CH0_IRQn);            //开启pit0中断
    }
    else if(PITN == 1)
    {
        pit_init_ms(pit1, ms);               //初始化pit1 周期设置为 X ms
        set_irq_priority(PIT_CH1_IRQn, 3);   //设置pit1优先级 0-3
        enable_irq(PIT_CH1_IRQn);            //开启pit1中断
    }
}

void User_ADC_Init(void) //ADC
{
    Detection_init(RR);  //RR
    Detection_init(RL);  //RL
    Detection_init(LR);  //LR
    Detection_init(LL);  //LL
}

void Start_Mode(void)              //启动模式
{
    Dial_Switch_Init();            //拨码开关
    Start_Mode_Value = Dial_Switch_Read();
    switch (Start_Mode_Value)
    {
    case 0:
        OLED_P6x8Str(121, 7, "0");
        User_Pit_Init(0,2);          //正常运行模式 控制周期10MS
        break;
    case 1:
        OLED_P6x8Str(121, 7, "1");
        User_Pit_Init(1,50);         //数值调试模式
        break;
    case 2:
        OLED_P6x8Str(121, 7, "2");
        ResetFlash();                //重置Flash
        OLED_P8x16Str(0, 0, "Complete Reset");
        OLED_P8x16Str(0, 2, "Please Restart");
        break;
    case 3:
        OLED_P6x8Str(121, 7, "3");
        Key_Init();                  //参数调试模式
        Debug = 1;
        break;
    default:
        OLED_P8x16Str(0, 0, "Err");
        break;
    }
    gpio_init(A2,GPO,0);
}
