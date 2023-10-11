#ifndef __MY_DMA_USART_H
#define __MY_DMA_USART_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>

#define USART_DMA_RECV_SIZE     (50)//���յĳ���
#define USART_RECV_SIZE  		 (50)//���յĳ���

/* ������������ */
typedef struct 
{
	/* ���ݽ��ձ�־ */
	uint8_t usart_recv_flag;
	/* ���ݽ������� */
	uint16_t usart_recv_len;
	/* DMA ���ݳ��� */
	uint16_t usart_dma_recv_len;
	/* ���ݽ��������� */
	uint8_t usart_recv_buffer [USART_RECV_SIZE];
	uint8_t usart_recv_buffer_2 [USART_RECV_SIZE];//����DMA���ͣ���ֹ����ǰ���������
	/* DMA ���ݻ����� */
	uint8_t usart_dma_recv_buffer [USART_DMA_RECV_SIZE];
}my_usart_type;

void hal_uart_recv_judge(void);
void hal_uart_recv_init(void);
void hal_uart_irq_prejudge(void);

#endif
