#include "my_dma_usart.h"

//uint16_t num = 0;

my_usart_type usart1_pc_type;
extern DMA_HandleTypeDef hdma_usart1_rx;

void USART1_IRQHandler(void)//串口一中断处理函数
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  hal_uart_irq_prejudge();
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	HAL_UART_Receive_DMA(&huart1 ,usart1_pc_type.usart_dma_recv_buffer ,USART_DMA_RECV_SIZE);
  /* USER CODE END USART1_IRQn 1 */
}

void hal_uart_recv_init(void)
{
	/* 1. 打开串口空闲中断 */
	__HAL_UART_ENABLE_IT(&huart1 ,UART_IT_IDLE );
	/* 2. 打开 DMA 接收 */
	HAL_UART_Receive_DMA(&huart1 ,usart1_pc_type.usart_dma_recv_buffer, USART_DMA_RECV_SIZE);
}
void hal_uart_irq_prejudge(void)
{
	uint32_t temp;
	/* 1. 判断空闲中断 */
	if(__HAL_UART_GET_FLAG(&huart1 , UART_FLAG_IDLE) != RESET)
	{
		/* 2. 清除中断 */
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		/* 3. 停止 DMA 接收 */
		HAL_UART_DMAStop(&huart1);
	    //清除 DMA 接收中断，只需读取一次状态寄存器和数据寄存器，即可实现
//		temp = huart1.Instance->ICR;
//		temp = huart1.Instance->RDR;

		/* 4. 获取 DMA 接收数据长度 : NDTR 为递减计数器，故 SIZE - NDTR*/
		temp = USART_DMA_RECV_SIZE -__HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		usart1_pc_type.usart_dma_recv_len = temp;
		/* 5. 串口回调函数 */
		HAL_UART_RxCpltCallback(&huart1);
	}
}	
void hal_uart_recv_judge(void)
{
		/* 1. 判断是否有完整数据 */
		if(usart1_pc_type.usart_recv_flag == 1)
		{
//		/* 2. 打印数据到指定串口 */
//			HAL_UART_Transmit(&huart1 ,usart1_pc_type.usart_recv_buffer ,
//			usart1_pc_type.usart_recv_len ,100 );
		/* 2. 对接收到的数据进行处理 */	
//			if(usart1_pc_type.usart_recv_buffer[0] == 0XA7 && usart1_pc_type.usart_recv_buffer[usart1_pc_type.usart_recv_len-1] == 0x7A)
//				//判断头尾是否为对应数据，如果是执行大括号内容
//			{			    
//				  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
//				  num ++ ;

//			   }
			/*将数据复制到usart_recv_buffer_2，方便DMA发送，如果不这么做在执行DMA发送时，清空接收缓存区会将数据清除导致发送的数据不全*/
			memcpy(usart1_pc_type.usart_recv_buffer_2 ,usart1_pc_type.usart_recv_buffer ,usart1_pc_type.usart_recv_len);
			
			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)usart1_pc_type.usart_recv_buffer_2, usart1_pc_type.usart_recv_len);//DMA发送

		/* 3. 清空接收缓存区 */
			memset(usart1_pc_type.usart_recv_buffer ,0 ,usart1_pc_type.usart_recv_len);//清除
		
		/* 4. 清空数据接收标志 */
			usart1_pc_type.usart_recv_flag = 0;
		
		/* 5. 清空数据接收长度 */
			usart1_pc_type.usart_recv_len = 0;			
		}
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* 1. 判断是否为 NB 串口 */
	if(huart->Instance == huart1.Instance)
	{
		/* 2. 判断是否有未处理的数据 */
		/* 有，从地址 0+old_len 开始获取 DMA 接收区数据。 */
		if(usart1_pc_type.usart_recv_len > 0)
		{
			memcpy(&usart1_pc_type.usart_recv_buffer[usart1_pc_type.usart_recv_len] ,\
			usart1_pc_type.usart_dma_recv_buffer ,usart1_pc_type.usart_dma_recv_len);
			
			usart1_pc_type.usart_recv_len += usart1_pc_type.usart_dma_recv_len;
		}
		/* 无，从地址 0 开始获取 DMA 接收区数据。 */
		else
		{
			memcpy(usart1_pc_type.usart_recv_buffer ,usart1_pc_type.usart_dma_recv_buffer ,\
			usart1_pc_type.usart_dma_recv_len);
			
			usart1_pc_type.usart_recv_len = usart1_pc_type.usart_dma_recv_len;
		}
		/* 3. 清空 DMA 接收区 */
		memset(usart1_pc_type.usart_dma_recv_buffer ,0 ,usart1_pc_type.usart_dma_recv_len);
		/* 4. 清空 DMA 接收长度 */
		usart1_pc_type.usart_dma_recv_len = 0;
		/* 5. 置位数据接收标志 */
		usart1_pc_type.usart_recv_flag = 1;
		
		/*此函数可以在任意外部处理，避免停留在中断时间过久*/
		hal_uart_recv_judge();
	}
}
