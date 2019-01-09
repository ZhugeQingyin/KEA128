#include "common.h"
#include "KEA128_port_cfg.h"
#include <stdio.h>
#include <math.h>
#include "SKEAZ1284.h"

//--------片上外设函数库-------
#include "KEA128_ftm.h"
#include "KEA128_pit.h"
#include "KEA128_gpio.h"
#include "KEA128_port.h"
#include "KEA128_adc.h"
#include "KEA128_irq.h"
#include "uart.h"
#include "KEA128_kbi.h"
#include "KEA128_flash.h"
#include "KEA128_systick.h"
#include "KEA128_i2c.h"
#include "KEA128_spi.h"
#include "KEA128_rtc.h"

//--------片外外设函数库--------
#include "Peripheral_FUN.h"
#include "Peripheral_IIC.h"
#include "Peripheral_MPU6050.h"
#include "Peripheral_OLED.h"
#include "diskio.h"
#include "ff.h"
#include "SD.h"

//----------USER函数库----------   
#include "Kalman.h"
#include "Stand.h"
#include "Speed.h"
#include "Motor.h"
#include "Detection.h"
#include "Interactive.h"
#include "Steer.h"
#include "isr.h"

//---------USER define----------
#define Debug_UART  UART_1
#define CMD_WARE         3     