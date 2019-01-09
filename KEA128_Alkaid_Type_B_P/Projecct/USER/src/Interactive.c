#include "headfile.h"
/*
 *人机交互大类
 */

typedef union {
    float fdata;
    unsigned long ldata;
} FloatLongType;

struct KEY
{
    unsigned char Menu;
    unsigned char Menu_Up;
    unsigned char Menu_Dn;
    unsigned char Value_Up;
    unsigned char Value_Dn;
} KEY_Flag;

struct Parameter
{
    float Stand_P;   // 1
    float Stand_D;   // 2
    float Speed_P;   // 3
    float Speed_I;   // 4
    float Steer_P_1; // 5
    float Steer_D_1; // 6
    float Steer_P_2; // 7
    float Steer_D_2; // 8
    float Steer_P_3; // 9
    float Steer_D_3; //10
    float Speed_VAL; //11
    float Angle_Set; //12
} Transfer_Data;

struct DATA
{
    int speed;           // 1
    int PWMOut_L;        // 2
    int PWMOut_R;        // 3
    int L_LL;            // 4
    int L_LR;            // 5
    int L_RL;            // 6
    int L_RR;            // 7
    int Angle;           // 8
    int Normalization_V; // 9
    int Normalization;   //10
    int fitting;         //11
    int Steer_D;         //12
    int Angle_Gyro;      //13
    int Angle_Calcu;     //14
    int RVL;             //15
    int Idle_flag;       //16
    int Gravity_flag;    //17
    int Ring_flag;       //18
    int Ring_Out;        //19
    int Runing_Time;     //20
} Run_Data;

void Transfer_Data_Init(void)
{
    unsigned char temp;
    temp = flash_read(254, 0, unsigned char);
    if (temp == 0xFF)
    {
        Transfer_Data.Stand_P = 610.0f;
        Transfer_Data.Stand_D = 560.0f;
        Transfer_Data.Speed_P = 40.0f;
        Transfer_Data.Speed_I = 0.2f;
        Transfer_Data.Steer_P_1 = 400.0f;
        Transfer_Data.Steer_D_1 = 3500.0f;
        Transfer_Data.Steer_P_2 = 450.0f;
        Transfer_Data.Steer_D_2 = 3300.0f;
        Transfer_Data.Steer_P_3 = 0.0f;
        Transfer_Data.Steer_D_3 = 0.0f;
        Transfer_Data.Speed_VAL = 220.0f;
        Transfer_Data.Angle_Set = 19.0f;
        WriteFlash();
        temp = 0;
        FLASH_EraseSector(254);
        FLASH_WriteSector(254, (const uint8 *)&temp, 1, 0);
    }
    else
    {
        ReadFlash();
    }
}

float Pass_Parameter(unsigned char Num)
{
    switch (Num)
    {
    case 1:
        return Transfer_Data.Stand_P;
    case 2:
        return Transfer_Data.Stand_D;
    case 3:
        return Transfer_Data.Speed_P;
    case 4:
        return Transfer_Data.Speed_I;
    case 5:
        return Transfer_Data.Steer_P_1;
    case 6:
        return Transfer_Data.Steer_D_1;
    case 7:
        return Transfer_Data.Steer_P_2;
    case 8:
        return Transfer_Data.Steer_D_2;
    case 9:
        return Transfer_Data.Steer_P_3;
    case 10:
        return Transfer_Data.Steer_D_3;
    case 11:
        return Transfer_Data.Speed_VAL;
    case 12:
        return Transfer_Data.Angle_Set;
    default:
        return 0.0f;
    }
}

void Run_Data_Init(void)
{
    Run_Data.Angle = 0;
    Run_Data.PWMOut_L = 0;
    Run_Data.PWMOut_R = 0;
    Run_Data.speed = 0;
    Run_Data.L_LL = 0;
    Run_Data.L_LR = 0;
    Run_Data.L_RL = 0;
    Run_Data.L_RR = 0;
    Run_Data.Normalization_V = 0;
    Run_Data.Normalization = 0;
    Run_Data.fitting = 0;
    Run_Data.Steer_D = 0;
    Run_Data.Idle_flag = 0;
    Run_Data.Gravity_flag = 0;
    Run_Data.Ring_flag = 0;
    Run_Data.Ring_Out = 0;
    Run_Data.Runing_Time = 0;
}

void Copy_Data(unsigned int Num, int value)
{
    switch (Num)
    {
    case 1:
        Run_Data.speed = value;
        break;
    case 2:
        Run_Data.PWMOut_L = value;
        break;
    case 3:
        Run_Data.PWMOut_R = value;
        break;
    case 4:
        Run_Data.L_LL = value;
        break;
    case 5:
        Run_Data.L_LR = value;
        break;
    case 6:
        Run_Data.L_RL = value;
        break;
    case 7:
        Run_Data.L_RR = value;
        break;
    case 8:
        Run_Data.Angle = value;
        break;
    case 9:
        Run_Data.Normalization_V = value;
        break;
    case 10:
        Run_Data.Normalization = value;
        break;
    case 11:
        Run_Data.fitting = value;
        break;
    case 12:
        Run_Data.Steer_D = value;
        break;
    case 13:
        Run_Data.Angle_Gyro = value;
        break;
    case 14:
        Run_Data.Angle_Calcu = value;
        break;
    case 15:
        Run_Data.RVL = value;
        break;
    case 16:
        Run_Data.Idle_flag = value;
        break;
    case 17:
        Run_Data.Gravity_flag = value;
        break;
    case 18:
        Run_Data.Ring_flag = value;
        break;
    case 19:
        Run_Data.Ring_Out = value;
        break;
    case 20:
        Run_Data.Runing_Time = value;
        break;
    default:
        break;
    }
}

int Read_Data(unsigned int Num)
{
    switch (Num)
    {
    case 1:
        return Run_Data.speed;
        break;
    case 2:
        return Run_Data.PWMOut_L;
        break;
    case 3:
        return Run_Data.PWMOut_R;
        break;
    case 4:
        return Run_Data.L_LL;
        break;
    case 5:
        return Run_Data.L_LR;
        break;
    case 6:
        return Run_Data.L_RL;
        break;
    case 7:
        return Run_Data.L_RR;
        break;
    case 8:
        return Run_Data.Angle;
        break;
    case 9:
        return Run_Data.Normalization_V;
        break;
    case 10:
        return Run_Data.Normalization;
        break;
    case 11:
        return Run_Data.fitting;
        break;
    case 12:
        return Run_Data.Steer_D;
        break;
    case 13:
        return Run_Data.Angle_Gyro;
        break;
    case 14:
        return Run_Data.Angle_Calcu;
        break;
    case 15:
        return Run_Data.RVL;
        break;
    case 16:
        return Run_Data.Idle_flag;
        break;
    case 17:
        return Run_Data.Gravity_flag;
        break;
    case 18:
        return Run_Data.Ring_flag;
    case 19:
        return Run_Data.Ring_Out;
    default:
        return 0;
        break;
    }
}

/*------------显示-------------*/
void Menu_0(void)
{
    //OLED_Print_Num1( 0, 0, Run_Data.Angle);
    OLED_Print_Num1(0, 0, Run_Data.L_LL);
    OLED_Print_Num1(36, 0, Run_Data.L_RR);
}
/*
Angle            speed
L_LL             L_RR
Normalization_V  Normalization  fitting
PWMOut_L         PWMOut_R       Steer_D
*/
void Menu_1(void)
{
    OLED_Print_Num1(0, 0, Run_Data.Angle);
    OLED_Print_Num1(36, 0, Run_Data.speed);

    OLED_Print_Num1(0, 1, Run_Data.L_LL);
    OLED_Print_Num1(36, 1, Run_Data.L_RR);

    OLED_Print_Num1(0, 2, Run_Data.Normalization_V);
    OLED_Print_Num1(36, 2, Run_Data.Normalization);
    OLED_Print_Num1(72, 2, Run_Data.fitting);

    OLED_Print_Num1(0, 3, -1 * Run_Data.PWMOut_L);
    OLED_Print_Num1(36, 3, -1 * Run_Data.PWMOut_R);
    OLED_Print_Num1(72, 3, Run_Data.Steer_D);
}
/*
L_LL L_RR
L_LR L_RL
*/
void Menu_2(void)
{
    OLED_Print_Num(0, 0, Run_Data.L_LL);
    OLED_Print_Num(48, 0, Run_Data.L_RR);

    OLED_Print_Num(0, 2, Run_Data.L_LR);
    OLED_Print_Num(48, 2, Run_Data.L_RL);

    OLED_Print_Num1(0, 4, Run_Data.Normalization);
    OLED_Print_Num1(36, 4, Run_Data.fitting);
    OLED_Print_Num1(72, 4, Run_Data.Normalization_V);
}

void Display(unsigned char Num)
{
    switch (Num)
    {
    case 0:
        Menu_0();
        break;
    case 1:
        Menu_1();
        break;
    case 2:
        Menu_2();
        break;
    default:
        break;
    }
}

/*----------蓝牙-----------*/

void Seed_Data(int value)
{
    char temp_value[3] = {0};
    sprintf(temp_value, "%d", value);
    uart_send_string(Debug_UART, temp_value);
}
/*L_LL, L_LR, L_RL, L_RR, Normalization, fitting, Ring_flag, Ring_Out, RVL, speed, Angle*/
void Send_0(void)
{
    Seed_Data(Run_Data.L_LL);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.L_LR);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.L_RL);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.L_RR);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Normalization);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.fitting);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Ring_flag);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Ring_Out);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.RVL);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.speed);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Angle);
    uart_send_string(Debug_UART, "\r\n");
}

void Send_1(void)
{
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE}; //串口调试 使用的前命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE}; //串口调试 使用的后命令

    uint8 t1 = Run_Data.L_LL;
    uint8 t2 = Run_Data.L_LR;
    uint8 t3 = Run_Data.L_RL;
    uint8 t4 = Run_Data.L_RR;

    uart_sendN(Debug_UART, sizeof(cmdf), cmdf);
    uart_send1(Debug_UART, t1);
    uart_send1(Debug_UART, t2);
    uart_send1(Debug_UART, t3);
    uart_send1(Debug_UART, t4);
    uart_sendN(Debug_UART, sizeof(cmdr), cmdr);
}

void Send_2(void)
{
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE}; //串口调试 使用的前命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE}; //串口调试 使用的后命令

    int16 t1 = Run_Data.Angle;
    int16 t2 = Run_Data.Angle_Gyro;
    int16 t3 = Run_Data.Angle_Calcu;

    uint8 byte1[2] = {0};
    uint8 byte2[2] = {0};
    uint8 byte3[2] = {0};

    byte1[0] = (unsigned char)t1;
    byte1[1] = (unsigned char)(t1 >> 8);

    byte2[0] = (unsigned char)t2;
    byte2[1] = (unsigned char)(t2 >> 8);

    byte3[0] = (unsigned char)t3;
    byte3[1] = (unsigned char)(t3 >> 8);

    uart_sendN(Debug_UART, sizeof(cmdf), cmdf);
    uart_sendN(Debug_UART, sizeof(byte1), byte1);
    uart_sendN(Debug_UART, sizeof(byte2), byte2);
    uart_sendN(Debug_UART, sizeof(byte3), byte3);
    uart_sendN(Debug_UART, sizeof(cmdr), cmdr);
}

void Send_3(void) //低密度发送
{
    static int Send_Cycle = 0;
    Send_Cycle++;
    if (Send_Cycle > 20)
    {
        Send_Cycle = 0;
        Send_0();
    }
}
/*L_LL, L_LR, L_RL, L_RR, Normalization, fitting, Ring_flag, Ring_Out, RVL, speed, Angle,Runing_Time*/
void Send_4(void)
{
    Seed_Data(Run_Data.L_LL);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.L_LR);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.L_RL);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.L_RR);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Normalization);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.fitting);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Ring_flag);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Ring_Out);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.RVL);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.speed);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Angle);
    uart_send_string(Debug_UART, ",");
    Seed_Data(Run_Data.Runing_Time);
    uart_send_string(Debug_UART, "\r\n");
}

void Send_Line(int value)
{
    Seed_Data(value);
    uart_send_string(Debug_UART, "\r\n");
}

void Bluetooth(unsigned char Num)
{
    switch (Num)
    {
    case 0:
        Send_0();
        break;
    case 1:
        Send_1();
        break;
    case 2:
        Send_2();
        break;
    case 3:
        Send_3();
        break;
    case 4:
        Send_4();
    default:
        break;
    }
}

/*----------按键与拨码开关-----------*/
void Dial_Switch_Init(void)
{
    gpio_init(E2, GPI, 0); //key_0
    gpio_init(E3, GPI, 0); //key_1
}
/*拨码开关读值*/
unsigned char Dial_Switch_Read(void)
{
    unsigned char key_0 = 0;
    unsigned char key_1 = 0;
    unsigned char out = 0;
    key_0 = gpio_get(E2);
    key_1 = gpio_get(E3);

    out = key_0 | (key_1 << 1);

    return out;
}

/*-------KBI中断按键函数--------*/
/*按键初始化*/
void Key_Init(void)
{
    KEY_Flag.Menu = 1;
    KEY_Flag.Menu_Up = 0;
    KEY_Flag.Menu_Dn = 0;
    KEY_Flag.Value_Up = 0;
    KEY_Flag.Value_Dn = 0;

    kbi_init(KBI1_P16, IRQ_FALLING_LOW);
    kbi_init(KBI1_P17, IRQ_FALLING_LOW);
    kbi_init(KBI1_P18, IRQ_FALLING_LOW);
    kbi_init(KBI1_P19, IRQ_FALLING_LOW);
    set_irq_priority(KBI1_IRQn, 1);
    enable_irq(KBI1_IRQn);
}

void Key_Menu_Up(void)
{
    KEY_Flag.Menu_Up = 1;
}

void Key_Menu_Dn(void)
{
    KEY_Flag.Menu_Dn = 1;
}

void Key_Value_Up(void)
{
    KEY_Flag.Value_Up = 1;
}

void Key_Value_Dn(void)
{
    KEY_Flag.Value_Dn = 1;
}

char Key_value_UpDn(void)
{
    if (KEY_Flag.Value_Up == 1)
    {
        KEY_Flag.Value_Up = 0;
        return 1;
    }
    if (KEY_Flag.Value_Dn == 1)
    {
        KEY_Flag.Value_Dn = 0;
        return -1;
    }
    return 0;
}

void Key_Dis(void)
{
    if (KEY_Flag.Menu_Up == 1)
    {
        if (KEY_Flag.Menu <= 12)
        {
            KEY_Flag.Menu++;
        }
        KEY_Flag.Menu_Up = 0;
    }
    if (KEY_Flag.Menu_Dn == 1)
    {
        if (KEY_Flag.Menu > 1)
        {
            KEY_Flag.Menu--;
        }
        KEY_Flag.Menu_Dn = 0;
    }
    switch (KEY_Flag.Menu)
    {
    case 1:
        OLED_P8x16Str(0, 0, "Stand_P  ");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Stand_P);
        Transfer_Data.Stand_P += (1.0f * Key_value_UpDn());
        break;
    case 2:
        OLED_P8x16Str(0, 0, "Stand_D  ");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Stand_D);
        Transfer_Data.Stand_D += (1.0f * Key_value_UpDn());
        break;
    case 3:
        OLED_P8x16Str(0, 0, "Speed_P  ");
        OLED_Print_Num(0, 2, (unsigned int)(Transfer_Data.Speed_P * 1));
        Transfer_Data.Speed_P += (1.0f * Key_value_UpDn());
        break;
    case 4:
        OLED_P8x16Str(0, 0, "Speed_I  ");
        OLED_Print_Num(0, 2, (unsigned int)(Transfer_Data.Speed_I * 100));
        Transfer_Data.Speed_I += (0.01f * Key_value_UpDn());
        break;
    case 5:
        OLED_P8x16Str(0, 0, "Steer_P_1");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Steer_P_1);
        Transfer_Data.Steer_P_1 += (5.0f * Key_value_UpDn());
        break;
    case 6:
        OLED_P8x16Str(0, 0, "Steer_D_1");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Steer_D_1);
        Transfer_Data.Steer_D_1 += (10.0f * Key_value_UpDn());
        break;
    case 7:
        OLED_P8x16Str(0, 0, "Steer_P_2");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Steer_P_2);
        Transfer_Data.Steer_P_2 += (5.0f * Key_value_UpDn());
        break;
    case 8:
        OLED_P8x16Str(0, 0, "Steer_D_2");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Steer_D_2);
        Transfer_Data.Steer_D_2 += (10.0f * Key_value_UpDn());
        break;
    case 9:
        OLED_P8x16Str(0, 0, "Steer_P_3");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Steer_P_3);
        Transfer_Data.Steer_P_3 += (5.0f * Key_value_UpDn());
        break;
    case 10:
        OLED_P8x16Str(0, 0, "Steer_D_3");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Steer_D_3);
        Transfer_Data.Steer_D_3 += (5.0f * Key_value_UpDn());
        break;
    case 11:
        OLED_P8x16Str(0, 0, "Speed_VAL");
        OLED_Print_Num(0, 2, (unsigned int)Transfer_Data.Speed_VAL);
        Transfer_Data.Speed_VAL += (5.0f * Key_value_UpDn());
        break;
    case 12:
        OLED_P8x16Str(0, 0, "Angle_Set");
        OLED_Print_Num(0, 2, (unsigned int)(Transfer_Data.Angle_Set * 10));
        Transfer_Data.Angle_Set += (0.1f * Key_value_UpDn());
        break;
    default:
        WriteFlash();
        KEY_Flag.Menu = 1;
        break;
    }
}

/*------Flash操作----*/

void Float_to_Byte(float f, unsigned char byte[]) //将浮点数f转化为4个字节数据存放在byte[4]中
{
    FloatLongType fl;
    fl.fdata = f;
    byte[0] = (unsigned char)fl.ldata;
    byte[1] = (unsigned char)(fl.ldata >> 8);
    byte[2] = (unsigned char)(fl.ldata >> 16);
    byte[3] = (unsigned char)(fl.ldata >> 24);
}

void Byte_to_Float(float *f, unsigned char byte[]) //将4个字节数据byte[4]转化为浮点数存放在*f中
{
    FloatLongType fl;
    fl.ldata = 0;
    fl.ldata = byte[3];
    fl.ldata = (fl.ldata << 8) | byte[2];
    fl.ldata = (fl.ldata << 8) | byte[1];
    fl.ldata = (fl.ldata << 8) | byte[0];
    *f = fl.fdata;
}

void WriteFlash(void)
{
    unsigned char Data_Flash[48] = {0};
    unsigned char Temp[4] = {0};

    Float_to_Byte(Transfer_Data.Stand_P, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[0 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Stand_D, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[4 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Speed_P, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[8 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Speed_I, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[12 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Steer_P_1, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[16 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Steer_D_1, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[20 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Steer_P_2, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[24 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Steer_D_2, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[28 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Steer_P_3, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[32 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Steer_D_3, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[36 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Speed_VAL, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[40 + i] = Temp[i];
    }

    Float_to_Byte(Transfer_Data.Angle_Set, Temp);
    for (unsigned char i = 0; i < 4; i++)
    {
        Data_Flash[44 + i] = Temp[i];
    }

    FLASH_EraseSector(255);
    FLASH_WriteSector(255, (const uint8 *)Data_Flash, 48, 0);
}

void ReadFlash(void)
{
    Transfer_Data.Stand_P = flash_read(255, 0, float);
    Transfer_Data.Stand_D = flash_read(255, 4, float);
    Transfer_Data.Speed_P = flash_read(255, 8, float);
    Transfer_Data.Speed_I = flash_read(255, 12, float);
    Transfer_Data.Steer_P_1 = flash_read(255, 16, float);
    Transfer_Data.Steer_D_1 = flash_read(255, 20, float);
    Transfer_Data.Steer_P_2 = flash_read(255, 24, float);
    Transfer_Data.Steer_D_2 = flash_read(255, 28, float);
    Transfer_Data.Steer_P_3 = flash_read(255, 32, float);
    Transfer_Data.Steer_D_3 = flash_read(255, 36, float);
    Transfer_Data.Speed_VAL = flash_read(255, 40, float);
    Transfer_Data.Angle_Set = flash_read(255, 44, float);
}

void ResetFlash(void)
{
    unsigned char temp = 0xFF;
    FLASH_EraseSector(254);
    FLASH_WriteSector(254, (const uint8 *)&temp, 1, 0);
}

/*-------停车------*/
void Stop_init(void)
{
    prot_pull(C6);
    prot_pull(C7);
    kbi_init(KBI0_P22, IRQ_FALLING);
    kbi_init(KBI0_P23, IRQ_FALLING);
    set_irq_priority(KBI0_IRQn, 1);
    enable_irq(KBI0_IRQn);

    PORT->IOFLT0 = PORT_IOFLT0_FLTDIV3(0x02);
    PORT->IOFLT0 = PORT_IOFLT0_FLTC(0x03);
    gpio_set(A2, 1);
}

void Stop(int value)
{
    /*停车*/
    DisableInterrupts;
    Motor_Stop();
    switch (value)
    {
    case 1:
        OLED_P8x16Str(0, 0, "Stop");
        OLED_P8x16Str(0, 2, "Finish_C6");
        break;
    case 2:
        OLED_P8x16Str(0, 0, "Stop");
        OLED_P8x16Str(0, 2, "Finish_C7");
        break;
    case 3:
        OLED_P8x16Str(0, 0, "Stop");
        OLED_P8x16Str(0, 2, "No Signal");
        break;
    default:
        //OLED_P8x16Str(0, 0, "Err");
        EnableInterrupts;
        break;
    }
}