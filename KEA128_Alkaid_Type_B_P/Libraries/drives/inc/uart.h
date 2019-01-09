﻿//============================================================================
//文件名称：uart.h
//功能概要：UART底层驱动构件头文件
//版权所有：苏州大学飞思卡尔嵌入式中心(sumcu.suda.edu.cn)
//版本更新：2015-06-05  V2.0
//芯片类型：KEA128
//============================================================================
#ifndef _UART_H      //防止重复定义（ 开头)
#define _UART_H

#include "common.h"   //包含公共要素头文件

//宏定义串口号
#define UART_0    0
#define UART_1    1
#define UART_2    2

//宏定义串口使用的引脚（由具体硬件板决定）
#define UART_0_GROUP   2    //UART_0: 1=PTA3~2脚,2=PTB1~0脚
#define UART_1_GROUP   2    //UART_1: 1=PTC7~6脚,2=PTF3~PTF2脚
#define UART_2_GROUP   1    //UART_2: 1=PTD7~6脚,2=PTI1~0脚


//===========================================================================
//函数名称：uart_init
//功能概要：使用总线时钟（BUS_CLK_KHZ），初始化uart模块
//参数说明：uartNo:串口号：UART_0、UART_1、UART_2 
//       baud:波特率：1200、2400、4800、9600、19200、115200
//函数返回：无
//===========================================================================
void uart_init(uint8 uartNo, uint32 baud_rate);

//===========================================================================
//函数名称：uart_send1
//参数说明：uartNo: 串口号:UART_0、UART_1、UART_2 
//       data:要发送的字节
//函数返回：函数执行状态：0=正常；非0=异常。
//功能概要：串行发送1个字节
//===========================================================================
uint8 uart_send1(uint8 uartNo, uint8 data);

//===========================================================================
//函数名称：uart_sendN
//参数说明：uartNo: 串口号:UART_0、UART_1、UART_2 
//      buff: 发送缓冲区
//      len:发送字节数
//函数返回： 函数执行状态：0=正常；1=异常。
//功能概要：串行发送多个字节
//===========================================================================
uint8 uart_sendN(uint8 uartNo, uint16 len, uint8* buff);

//===========================================================================
//函数名称：uart_send_string
//参数说明：uartNo:UART模块号:UART_0、UART_1、UART_2 
//          buff:要发送的字符串的首地址
//函数返回： 函数执行状态：0=正常；非0=异常。
//功能概要：从指定UART端口发送一个以'\0'结束的字符串(以双引号表达的字符串，即是以'\0'结束的字符串）
//例：uart_send_string(UART_0,"abcdefg"); 即可发送字符  abcdefg
//===========================================================================
uint8 uart_send_string(uint8 uartNo, int8 *buff);

//===========================================================================
//函数名称：uart_re1
//参数说明：uartNo: 串口号:UART_0、UART_1、UART_2 
//          fp:接收成功标志的指针:*fp=0，成功接收；*fp=1，接收失败
//函数返回：接收返回字节
//功能概要：串行接收1个字节
//===========================================================================
uint8 uart_re1(uint8 uartNo, uint8 *fp);

//===========================================================================
//函数名称：uart_reN
//参数说明：uartNo: 串口号:UART_0、UART_1、UART_2                                                   
//          buff: 接收缓冲区
//          len:接收长度
//函数返回：函数执行状态 0=正常;非0=异常
//功能概要：串行 接收n个字节
//===========================================================================
uint8 uart_reN(uint8 uartNo, uint16 len, uint8* buff);

////===========================================================================
////函数名称：uart_enable_re_int
////参数说明：uartNo: 串口号:UART_0、UART_1、UART_2 
////函数返回：无
////功能概要：开串口接收中断
////===========================================================================
//void uart_enable_re_int(uint8 uartNo);
//
////===========================================================================
////函数名称：uart_disable_re_int
////参数说明：uartNo: 串口号 :UART_0、UART_1、UART_2 
////函数返回：无
////功能概要：关串口接收中断
////===========================================================================
//void uart_disable_re_int(uint8 uartNo);
#endif        //防止重复定义（ 结尾)

//===========================================================================
//声明：
//（1）我们开发的源代码，在本中心提供的硬件系统测试通过，真诚奉献给社会，不足之处，欢迎指正。
//（2）对于使用非本中心硬件系统的用户，移植代码时，请仔细根据自己的硬件匹配。
//
//苏州大学飞思卡尔嵌入式中心
//技术咨询：0512-65214835  http://sumcu.suda.edu.cn
