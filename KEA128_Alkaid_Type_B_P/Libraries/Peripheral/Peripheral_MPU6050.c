#include "Peripheral_MPU6050.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void InitMPU6050(void)
{
  simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	     //解除休眠状态
  simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x4F);      //125HZ采样率
  simiic_write_reg(MPU6050_DEV_ADDR, CONFIG, 0x00);          //LPF 44HZ
  simiic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);     //+-2000Deg/s
  simiic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x18);    //+-16g
  simiic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
  simiic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
  simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x01);
}




//内部使用用户无需调用
int16 GetData(uint8 REG_Address)
{
  uint8 L;   uint16 H ;
  H=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address, IIC);
  L=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address+1, IIC);
  return (H<<8)+L;   //合成数据
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  int mpu_acc_x,mpu_acc_y,mpu_acc_z;
//  Get_AccData(&mpu_acc_x,&mpu_acc_y,&mpu_acc_z);
//执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_AccData(int *mpu_acc_x,int *mpu_acc_y,int *mpu_acc_z)
{
  *mpu_acc_x = GetData(ACCEL_XOUT_H);
  *mpu_acc_y = GetData(ACCEL_YOUT_H);
  *mpu_acc_z = GetData(ACCEL_ZOUT_H);
}


/*-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	
int mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
Get_Gyro(&mpu_gyro_x,&mpu_gyro_y,&mpu_gyro_z);
//执行该函数后，直接查看对应的变量即可
-------------------------------------------------------------------------------------------------------------------*/
void Get_Gyro(int *mpu_gyro_x,int *mpu_gyro_y,int *mpu_gyro_z)
{
  *mpu_gyro_x = GetData(GYRO_XOUT_H);
  *mpu_gyro_y = GetData(GYRO_YOUT_H);
  *mpu_gyro_z = GetData(GYRO_ZOUT_H);
}


