//port.h/*
/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#ifndef _PORT_H
#define _PORT_H

#include <assert.h>
#include <inttypes.h>

#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( ) __disable_irq()
#define EXIT_CRITICAL_SECTION( )  __enable_irq()

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef char CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

//定时器，串口通信设置，如使用huart1则将所有数字改成1，定时器同上
#define Modbus_Rtu_serial huart1  //Modbus通讯的串口
#define Modbus_Rtu_huart_number USART1  //Modbus通讯的串口
#define Modbus_Rtu_IRQHandler USART1_IRQHandler //串口中断
#define Modbus_Rtu_IRQ USART1_IRQn //标志位

#define Modbus_Rtu_timer htim7 //modbus使用的定时器
#define Modbus_Rtu_timer_number TIM7 //modbus使用的定时器
#define Modbus_Rtu_timer_prescaler 8399//预分配系数，要求定时50us
#define Modbus_Rtu_timer_IRQHandler TIM7_IRQHandler

#endif
