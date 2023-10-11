#include "my_dma_usart.h"

//uint16_t num = 0;

my_usart_type usart1_pc_type;
extern DMA_HandleTypeDef hdma_usart1_rx;

void USART1_IRQHandler(void)//����һ�жϴ�����
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
	/* 1. �򿪴��ڿ����ж� */
	__HAL_UART_ENABLE_IT(&huart1 ,UART_IT_IDLE );
	/* 2. �� DMA ���� */
	HAL_UART_Receive_DMA(&huart1 ,usart1_pc_type.usart_dma_recv_buffer, USART_DMA_RECV_SIZE);
}
void hal_uart_irq_prejudge(void)
{
	uint32_t temp;
	/* 1. �жϿ����ж� */
	if(__HAL_UART_GET_FLAG(&huart1 , UART_FLAG_IDLE) != RESET)
	{
		/* 2. ����ж� */
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		/* 3. ֹͣ DMA ���� */
		HAL_UART_DMAStop(&huart1);
	    //��� DMA �����жϣ�ֻ���ȡһ��״̬�Ĵ��������ݼĴ���������ʵ��
//		temp = huart1.Instance->ICR;
//		temp = huart1.Instance->RDR;

		/* 4. ��ȡ DMA �������ݳ��� : NDTR Ϊ�ݼ����������� SIZE - NDTR*/
		temp = USART_DMA_RECV_SIZE -__HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		usart1_pc_type.usart_dma_recv_len = temp;
		/* 5. ���ڻص����� */
		HAL_UART_RxCpltCallback(&huart1);
	}
}	
void hal_uart_recv_judge(void)
{
		/* 1. �ж��Ƿ����������� */
		if(usart1_pc_type.usart_recv_flag == 1)
		{
//		/* 2. ��ӡ���ݵ�ָ������ */
//			HAL_UART_Transmit(&huart1 ,usart1_pc_type.usart_recv_buffer ,
//			usart1_pc_type.usart_recv_len ,100 );
		/* 2. �Խ��յ������ݽ��д��� */	
//			if(usart1_pc_type.usart_recv_buffer[0] == 0XA7 && usart1_pc_type.usart_recv_buffer[usart1_pc_type.usart_recv_len-1] == 0x7A)
//				//�ж�ͷβ�Ƿ�Ϊ��Ӧ���ݣ������ִ�д���������
//			{			    
//				  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
//				  num ++ ;

//			   }
			/*�����ݸ��Ƶ�usart_recv_buffer_2������DMA���ͣ��������ô����ִ��DMA����ʱ����ս��ջ������Ὣ����������·��͵����ݲ�ȫ*/
			memcpy(usart1_pc_type.usart_recv_buffer_2 ,usart1_pc_type.usart_recv_buffer ,usart1_pc_type.usart_recv_len);
			
			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)usart1_pc_type.usart_recv_buffer_2, usart1_pc_type.usart_recv_len);//DMA����

		/* 3. ��ս��ջ����� */
			memset(usart1_pc_type.usart_recv_buffer ,0 ,usart1_pc_type.usart_recv_len);//���
		
		/* 4. ������ݽ��ձ�־ */
			usart1_pc_type.usart_recv_flag = 0;
		
		/* 5. ������ݽ��ճ��� */
			usart1_pc_type.usart_recv_len = 0;			
		}
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* 1. �ж��Ƿ�Ϊ NB ���� */
	if(huart->Instance == huart1.Instance)
	{
		/* 2. �ж��Ƿ���δ��������� */
		/* �У��ӵ�ַ 0+old_len ��ʼ��ȡ DMA ���������ݡ� */
		if(usart1_pc_type.usart_recv_len > 0)
		{
			memcpy(&usart1_pc_type.usart_recv_buffer[usart1_pc_type.usart_recv_len] ,\
			usart1_pc_type.usart_dma_recv_buffer ,usart1_pc_type.usart_dma_recv_len);
			
			usart1_pc_type.usart_recv_len += usart1_pc_type.usart_dma_recv_len;
		}
		/* �ޣ��ӵ�ַ 0 ��ʼ��ȡ DMA ���������ݡ� */
		else
		{
			memcpy(usart1_pc_type.usart_recv_buffer ,usart1_pc_type.usart_dma_recv_buffer ,\
			usart1_pc_type.usart_dma_recv_len);
			
			usart1_pc_type.usart_recv_len = usart1_pc_type.usart_dma_recv_len;
		}
		/* 3. ��� DMA ������ */
		memset(usart1_pc_type.usart_dma_recv_buffer ,0 ,usart1_pc_type.usart_dma_recv_len);
		/* 4. ��� DMA ���ճ��� */
		usart1_pc_type.usart_dma_recv_len = 0;
		/* 5. ��λ���ݽ��ձ�־ */
		usart1_pc_type.usart_recv_flag = 1;
		
		/*�˺��������������ⲿ��������ͣ�����ж�ʱ�����*/
		hal_uart_recv_judge();
	}
}
