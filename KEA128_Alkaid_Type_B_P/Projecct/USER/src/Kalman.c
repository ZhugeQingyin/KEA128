#include "headfile.h"
/*写作Kalman读作互补滤波*/
float Kalman(int Gyro_x, int Gyro_y, int Gyro_z, int Accel_x, int Accel_y, int Accel_z)
{
    static float Angle = 38.0f;
    float Angle_Calcu  = 0.0f;
    float Gyro_x_Calcu = 0.0f;
    float Q_Accel      = 0.045f;      //加速度计可信度参数
    int Gyro_x_calibration  = 34;     //角加速度 X轴零点修正
    int Accel_y_calibration = 37;     //加速度   Y轴    修正
    int Accel_z_calibration = 268;    //加速度   Z轴    修正
    Accel_y += Accel_y_calibration;
    Accel_z += Accel_z_calibration;
    Gyro_x_Calcu = -1.0f * (Gyro_x + Gyro_x_calibration) / 16.4f;
    Gyro_x_Calcu = Angle + Gyro_x_Calcu * 0.01f;
    Copy_Data(13,(int)(Gyro_x_Calcu * 10));
    
    Accel_y = Accel_y > -700 ? -700 : Accel_y;
    Accel_z = Accel_z > -300 ? -300 : Accel_z;

    Angle_Calcu = (1.0f * Accel_z) / (1.0f * Accel_y);
    Angle_Calcu = atan(Angle_Calcu) * 57.295f;
    Copy_Data(14,(int)(Angle_Calcu) * 10);
    
    Angle = (1.0f - Q_Accel) * (Gyro_x_Calcu) + Q_Accel * Angle_Calcu;
    Copy_Data(8, (int)(Angle * 10));
    return Angle;
}

int Gravity(int Accel_x, int Accel_y, int Accel_z)
{
    static int AVR_Time = 10;
    static float AVR_G = 0.0f;
    static float Threshold_G = 0;
    float Temp_G = 0;
    if(AVR_Time >= 0)
    {
        if(AVR_Time > 0)
        {
            AVR_G += (Accel_z / 2048) * (Accel_z / 2048) + (Accel_y / 2048) * (Accel_y / 2048);
        }
        else
        {
            AVR_G /= 10;
            Threshold_G = 0.05 * AVR_G;
        }
        return 0;
    }
    Temp_G = (Accel_z / 2048) * (Accel_z / 2048) + (Accel_y / 2048) * (Accel_y / 2048);
    if((Temp_G - AVR_G) > (AVR_G + Threshold_G))
    {
        return 1;
    }
    else if((Temp_G - AVR_G) < (AVR_G - Threshold_G))
    {
        return -1;
    }
    else 
    {
        return 0;
    }
}

void Kalman_Send(int Gyro_x, int Gyro_y, int Gyro_z, int Accel_x, int Accel_y, int Accel_z)
{
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令
    
    int16 t1 = Gyro_x;
    int16 t2 = Accel_y;
    int16 t3 = Accel_z;
    
    uint8 byte1[2] = { 0 };
    uint8 byte2[2] = { 0 };
    uint8 byte3[2] = { 0 };
    
    byte1[0] = (unsigned char)t1;
    byte1[1] = (unsigned char)(t1>>8);
    
    byte2[0] = (unsigned char)t2;
    byte2[1] = (unsigned char)(t2>>8);
    
    byte3[0] = (unsigned char)t3;
    byte3[1] = (unsigned char)(t3>>8);  
    
    uart_sendN(Debug_UART,sizeof(cmdf),cmdf);
    uart_sendN(Debug_UART,sizeof(byte1),byte1);
    uart_sendN(Debug_UART,sizeof(byte2),byte2);
    uart_sendN(Debug_UART,sizeof(byte3),byte3);
    uart_sendN(Debug_UART,sizeof(cmdr),cmdr);  
}

