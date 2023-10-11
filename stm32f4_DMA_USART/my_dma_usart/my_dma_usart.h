#ifndef __MY_DMA_USART_H
#define __MY_DMA_USART_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>

#define USART_DMA_RECV_SIZE     (50)//接收的长度
#define USART_RECV_SIZE  		 (50)//接收的长度

/* 串口数据类型 */
typedef struct 
{
	/* 数据接收标志 */
	uint8_t usart_recv_flag;
	/* 数据解析长度 */
	uint16_t usart_recv_len;
	/* DMA 数据长度 */
	uint16_t usart_dma_recv_len;
	/* 数据解析缓冲区 */
	uint8_t usart_recv_buffer [USART_RECV_SIZE];
	uint8_t usart_recv_buffer_2 [USART_RECV_SIZE];//用于DMA发送，防止发送前被清除数据
	/* DMA 数据缓冲区 */
	uint8_t usart_dma_recv_buffer [USART_DMA_RECV_SIZE];
}my_usart_type;

void hal_uart_recv_judge(void);
void hal_uart_recv_init(void);
void hal_uart_irq_prejudge(void);

#endif
