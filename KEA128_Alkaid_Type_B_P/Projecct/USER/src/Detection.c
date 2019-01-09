#include "headfile.h"
/*
ADC
引脚  通道
PTA0  SE0
PTA1  SE1
PTA6  SE2
PTA7  SE3
PTB0  SE4
PTB1  SE5
PTB2  SE6
PTB3  SE7
----------
PTC0  SE8
PTC1  SE9
PTC2  SE10
PTC3  SE11
PTF4  SE12
PTF5  SE13
PTF6  SE14
PTF7  SE15
*/
int less(unsigned int a, unsigned int b)
{
    return a < b ? 1 : 0;
}

void exch(unsigned int *a, unsigned int *b)
{
    int temp = 0;
    temp = *a;
    *a = *b;
    *b = temp;
}

unsigned int MidAvr(unsigned int value[], int size)
{
    int i = 0;
    int j = 0;
    int sum = 0;
    for (i = 0; i < size; i++)
    {
        int min = i;
        for (j = i + 1; j <= size; j++)
        {
            if (less(value[j], value[min]))
                min = j;
        }
        exch(&value[i], &value[min]);
    }
    for (i = 1; i <= (size - 1); i++)
    {
        sum += value[i];
    }
    return (unsigned int)(sum / (size - 2));
}

int Detection_init(int SE)
{
    if (SE > 15)
        SE = 15;
    switch (SE)
    {
    case 15:
        adc_init(ADC0_SE15);
    case 14:
        adc_init(ADC0_SE14);
    case 13:
        adc_init(ADC0_SE13);
    case 12:
        adc_init(ADC0_SE12);
    case 11:
        adc_init(ADC0_SE11);
    case 10:
        adc_init(ADC0_SE10);
    case 9:
        adc_init(ADC0_SE9);
    case 8:
        adc_init(ADC0_SE8);
    case 7:
        adc_init(ADC0_SE7);
    case 6:
        adc_init(ADC0_SE6);
    case 5:
        adc_init(ADC0_SE5);
    case 4:
        adc_init(ADC0_SE4);
    case 3:
        adc_init(ADC0_SE3);
    case 2:
        adc_init(ADC0_SE2);
    case 1:
        adc_init(ADC0_SE1);
    case 0:
        adc_init(ADC0_SE0);
        break;
    default:
        return 0;
    }
    return 1;
}

unsigned int Detection_Once(int SE)
{
    unsigned int value = 0;
    if (SE > 15)
    {
        SE = 15;        
    }
    switch (SE)
    {
    case 15:
        value = adc_once(ADC0_SE15, ADC_12bit);
        break;
    case 14:
        value = adc_once(ADC0_SE14, ADC_12bit);
        break;
    case 13:
        value = adc_once(ADC0_SE13, ADC_12bit);
        break;
    case 12:
        value = adc_once(ADC0_SE12, ADC_12bit);
        break;
    case 11:
        value = adc_once(ADC0_SE11, ADC_12bit);
        break;
    case 10:
        value = adc_once(ADC0_SE10, ADC_12bit);
        break;
    case 9:
        value = adc_once(ADC0_SE9, ADC_12bit);
        break;
    case 8:
        value = adc_once(ADC0_SE8, ADC_12bit);
        break;
    case 7:
        value = adc_once(ADC0_SE7, ADC_12bit);
        break;
    case 6:
        value = adc_once(ADC0_SE6, ADC_12bit);
        break;
    case 5:
        value = adc_once(ADC0_SE5, ADC_12bit);
        break;
    case 4:
        value = adc_once(ADC0_SE4, ADC_12bit);
        break;
    case 3:
        value = adc_once(ADC0_SE3, ADC_12bit);
        break;
    case 2:
        value = adc_once(ADC0_SE2, ADC_12bit);
        break;
    case 1:
        value = adc_once(ADC0_SE1, ADC_12bit);
        break;
    case 0:
        value = adc_once(ADC0_SE0, ADC_12bit);
        break;
    default:
        return 0;
    }
    return value;
}
int Detection_MidAvr(int SE)
{
    unsigned int value = 0;
    unsigned int temp[12] = {0};
    if (SE > 15)
    {
        SE = 15;
    }
    switch (SE)
    {
    case 15:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE15, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 14:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE14, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 13:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE13, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 12:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE12, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 11:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE11, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 10:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE10, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 9:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE9, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 8:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE8, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 7:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE7, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 6:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE6, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 5:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE5, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 4:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE4, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 3:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE3, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 2:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE2, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 1:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE1, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    case 0:
        {
            for (int i = 0; i < 12; i++)
            {
                temp[i] = adc_once(ADC0_SE0, ADC_12bit);
            }
            value = MidAvr(temp, 12);
        }
        break;
        
    default:
        {
            return 0;
        }
    }
    return value;
}

