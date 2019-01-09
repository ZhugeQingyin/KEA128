#include "headfile.h"

void Motor_Init()
{
    ftm_pwm_init(ftm2, ftm_ch0, 10 * 1000, 0);
    ftm_pwm_init(ftm2, ftm_ch1, 10 * 1000, 0);
    ftm_pwm_init(ftm2, ftm_ch2, 10 * 1000, 0);
    ftm_pwm_init(ftm2, ftm_ch3, 10 * 1000, 0);
}

void Motor_L(int value)
{
    if (value >= 0)
    {
        ftm_pwm_duty(ftm2, ftm_ch0, value);
        ftm_pwm_duty(ftm2, ftm_ch1, 0);
    }
    else if(value < 0)
    {
        value *= -1;
        ftm_pwm_duty(ftm2, ftm_ch0, 0);
        ftm_pwm_duty(ftm2, ftm_ch1, value);
    }
}

void Motor_R(int value)
{
    if (value >= 0)
    {
        ftm_pwm_duty(ftm2, ftm_ch2, value);
        ftm_pwm_duty(ftm2, ftm_ch3, 0);
    }
    else if(value < 0)
    {
        value *= -1;
        ftm_pwm_duty(ftm2, ftm_ch2, 0);
        ftm_pwm_duty(ftm2, ftm_ch3, value);
    }
}

void Motor_Stop(void)
{
    ftm_pwm_duty(ftm2, ftm_ch0, 0);
    ftm_pwm_duty(ftm2, ftm_ch1, 0);
    ftm_pwm_duty(ftm2, ftm_ch2, 0);
    ftm_pwm_duty(ftm2, ftm_ch3, 0);
}