#ifndef _Kalman_H_
#define _Kalman_H_

float Kalman(int Gyro_x,int Gyro_y,int Gyro_z,int Accel_x,int Accel_y,int Accel_z); //直立滤波
void Kalman_Send(int Gyro_x, int Gyro_y, int Gyro_z, int Accel_x, int Accel_y, int Accel_z);
int Gravity(int Accel_x, int Accel_y, int Accel_z);
#endif