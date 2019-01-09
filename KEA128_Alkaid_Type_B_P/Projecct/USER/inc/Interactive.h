#ifndef _KEY_H_
#define _KEY_H_

/*以下宏定义为KBI中断用*/
/*KBI0*/
#define Stop_C6 0xFFBFFFFF
#define Stop_C7 0xFF7FFFFF
/*KBI1*/
#define K6 0xFFFEFFFF 
#define K5 0xFFFDFFFF 
#define K4 0xFFFBFFFF 
#define K3 0xFFF7FFFF

/*Copy_Data与Read_Data 映射*/
#define Num_speed             1
#define Num_PWMOut_L          2
#define Num_PWMOut_R          3
#define Num_L_LL              4
#define Num_L_LR              5
#define Num_L_RL              6
#define Num_L_RR              7
#define Num_Angle             8
#define Num_Normalization_V   9
#define Num_Normalization     10
#define Num_fitting           11
#define Num_Steer_D           12
#define Num_Angle_Gyro        13
#define Num_Angle_Calcu       14
#define Num_RVL               15
#define Num_Idle_flag         16
#define Num_Gravity_flag      17
#define Num_Ring_flag         18
#define Num_Ring_Out          19
#define Num_Runing_Time       20

/*拨码开关*/
void Dial_Switch_Init(void);                //两位拨码开关初始化
unsigned char Dial_Switch_Read(void);       //两位拨码开关读值

/*按键*/
void Key_Init(void);                        //按键初始化
void Key_Menu_Up(void);                     //按键 菜单上翻
void Key_Menu_Dn(void);                     //按键 菜单下翻
void Key_Value_Up(void);                    //按键 当前值增
void Key_Value_Dn(void);                    //按键 当前值减
void Key_Dis(void);                         //按键 菜单显示

/*数据移动*/
void Transfer_Data_Init(void);              //控制参数 初始化
void Run_Data_Init(void);                   //运行数据 初始化
void Copy_Data(unsigned int Num, int value);//运行数据 采集
int Read_Data(unsigned int Num);            //运行数据 读取
float Pass_Parameter(unsigned char Num);    //控制参数 传递
/*数据发送与显示*/
void Seed_Data(int value);                  //发送数据 int -> *char
void Send_Line(int value);                  //发送数据带回车(/r/n)
void Bluetooth(unsigned char Num);          //蓝牙发送模式选择
void Display(unsigned char Num);            //OLED显示模式选择

/*Flash*/
void WriteFlash(void);                      //Flash 写
void ReadFlash(void);                       //Flash 读 
void ResetFlash(void);                      //Flash 重置

/*停车*/
void Stop_init(void);                       //停车初始化
void Stop(int value);                       //停车

#endif
