#include "main.h"
#include "usart.h"
//#include "cmsis_os.h"
#include "my_uart.h"



#if USART1_EN
unsigned char USART1_UserBuffer[USART1_RXBUF_LEN];
unsigned char USART1_Buffer[USART1_RXBUF_LEN];
struct UART_REC_TYPE Uart1;

/**************************************
�������� uint8_t USART1_RecService(void)
˵  ���� ����1���շ�������ж�������û�н�����
��  ���� ��
����ֵ�� 1���������		0������δ��ɻ�������
**************************************/
uint8_t USART1_RecService(void)		
{
	static uint16_t USART1_RecLen_Last=0; 
	if(Uart1.RecCount)					//�������н���,���ٽ����һ�δ��ڽ����ж�
	{
//		#if Freertos_EN == 0		//�ȴ�2ms,������9600
//		HAL_Delay(2);
//		#endif
//		#if Freertos_EN == 1
//		osDelay(2);
//		#endif
		if(USART1_RecLen_Last == Uart1.RecCount)	//�жϽ������ݳ����Ƿ����ϴ�ѭ����ȣ������������������
		{
			USART1_RecLen_Last=0;
			Uart1.LastLen = Uart1.RecCount;					//��ȡ���ճ���
			Uart1.RecCount = 0;											//���´�Buff[0]��ʼ����
			for(uint16_t i=0;i<Uart1.LastLen;i++)  USART1_UserBuffer[i] = USART1_Buffer[i];		//�滻Buffer������Buffer��Ȼ��Ϊ�����ж���ʹ��			
			return 1;		
		}
		else USART1_RecLen_Last = Uart1.RecCount;
	}
	return 0;
}
#endif


#if USART2_EN
unsigned char USART2_UserBuffer[USART2_RXBUF_LEN];
unsigned char USART2_Buffer[USART2_RXBUF_LEN];
struct UART_REC_TYPE Uart2;

/**************************************
�������� uint8_t USART2_RecService(void)
˵  ���� ����2���շ�������ж�������û�н�����
��  ���� ��
����ֵ�� 1���������		0������δ��ɻ�������
**************************************/
uint8_t USART2_RecService(void)		
{
	static uint16_t USART2_RecLen_Last=0;
	if(Uart2.RecCount)					//�������н���,���ٽ����һ�δ��ڽ����ж�
	{
//		#if Freertos_EN == 0		//�ȴ�2ms,������9600
//		HAL_Delay(2);
//		#endif
//		#if Freertos_EN == 1
//		osDelay(2);
//		#endif
		if(USART2_RecLen_Last == Uart2.RecCount)	//�жϽ������ݳ����Ƿ����ϴ�ѭ����ȣ������������������
		{
			Uart2.LastLen = Uart2.RecCount;					//��ȡ���ճ���
			Uart2.RecCount = 0;											//���´�Buff[0]��ʼ����
			for(uint16_t i=0;i<Uart2.LastLen;i++)  USART2_UserBuffer[i] = USART2_Buffer[i];		//�滻Buffer������Buffer��Ȼ��Ϊ�����ж���ʹ��			
			return 1;		
		}
		else USART2_RecLen_Last = Uart2.RecCount;
	}
	return 0;
}
#endif

#if USART3_EN
unsigned char USART3_UserBuffer[USART3_RXBUF_LEN];
unsigned char USART3_Buffer[USART3_RXBUF_LEN];
struct UART_REC_TYPE Uart3;

/**************************************
�������� uint8_t USART3_RecService(void)
˵  ���� ����3���շ�������ж�������û�н�����
��  ���� ��
����ֵ�� 1���������		0������δ��ɻ�������
**************************************/
uint8_t USART3_RecService(void)		
{
	static uint16_t USART3_RecLen_Last=0;
	if(Uart3.RecCount)					//�������н���,���ٽ����һ�δ��ڽ����ж�
	{		
//		#if Freertos_EN == 0		//�ȴ�2ms,������9600
//		HAL_Delay(2);
//		#endif
//		#if Freertos_EN == 1
//		osDelay(2);
//		#endif
		if(USART3_RecLen_Last == Uart3.RecCount)	//�жϽ������ݳ����Ƿ����ϴ�ѭ����ȣ������������������
		{
			Uart3.LastLen = Uart3.RecCount;					//��ȡ���ճ���
			Uart3.RecCount = 0;											//���´�Buff[0]��ʼ����
			for(uint16_t i=0;i<Uart3.LastLen;i++)  USART3_UserBuffer[i] = USART3_Buffer[i];		//�滻Buffer������Buffer��Ȼ��Ϊ�����ж���ʹ��			
			return 1;		
		}
		else USART3_RecLen_Last = Uart3.RecCount;
	}
	return 0;
}
#endif

#if USART4_EN
unsigned char USART4_UserBuffer[USART4_RXBUF_LEN];
unsigned char USART4_Buffer[USART4_RXBUF_LEN];
struct UART_REC_TYPE Uart4;

/**************************************
�������� uint8_t USART4_RecService(void)
˵  ���� ����4���շ�������ж�������û�н�����
��  ���� ��
����ֵ�� 1���������		0������δ��ɻ�������
**************************************/
uint8_t USART4_RecService(void)		
{
	static uint16_t USART4_RecLen_Last=0;
	if(Uart4.RecCount)					//�������н���,���ٽ����һ�δ��ڽ����ж�
	{		
//		#if Freertos_EN == 0		//�ȴ�1ms,������115200
//		HAL_Delay(2);
//		#endif
//		#if Freertos_EN == 1
//		osDelay(2);
//		#endif
		if(USART4_RecLen_Last == Uart4.RecCount)	//�жϽ������ݳ����Ƿ����ϴ�ѭ����ȣ������������������
		{
			Uart4.LastLen = Uart4.RecCount;					//��ȡ���ճ���
			Uart4.RecCount = 0;											//���´�Buff[0]��ʼ����
			for(uint16_t i=0;i<Uart4.LastLen;i++)  USART4_UserBuffer[i] = USART4_Buffer[i];		//�滻Buffer������Buffer��Ȼ��Ϊ�����ж���ʹ��			
			return 1;		
		}
		else USART4_RecLen_Last = Uart4.RecCount;
	}
	return 0;
}
#endif

#if USART5_EN
unsigned char USART5_UserBuffer[USART5_RXBUF_LEN];
unsigned char USART5_Buffer[USART5_RXBUF_LEN];
struct UART_REC_TYPE Uart5;

/**************************************
�������� uint8_t USART5_RecService(void)
˵  ���� ����5���շ�������ж�������û�н�����
��  ���� ��
����ֵ�� 1���������		0������δ��ɻ�������
**************************************/
uint8_t USART5_RecService(void)		
{
	static uint16_t USART5_RecLen_Last=0;
	if(Uart5.RecCount)					//�������н���,���ٽ����һ�δ��ڽ����ж�
	{
//		#if Freertos_EN == 0		//�ȴ�1ms,������115200
//		HAL_Delay(2);
//		#endif
//		#if Freertos_EN == 1
//		osDelay(2);
//		#endif
		if(USART5_RecLen_Last == Uart5.RecCount)	//�жϽ������ݳ����Ƿ����ϴ�ѭ����ȣ������������������
		{
			Uart5.LastLen = Uart5.RecCount;					//��ȡ���ճ���
			Uart5.RecCount = 0;											//���´�Buff[0]��ʼ����
			for(uint16_t i=0;i<Uart5.LastLen;i++)  USART5_UserBuffer[i] = USART5_Buffer[i];		//�滻Buffer������Buffer��Ȼ��Ϊ�����ж���ʹ��			
			return 1;		
		}
		else	USART5_RecLen_Last = Uart5.RecCount;
	}
	return 0;
}
#endif

#if USART6_EN
unsigned char USART6_UserBuffer[USART2_RXBUF_LEN];
unsigned char USART6_Buffer[USART2_RXBUF_LEN];
struct UART_REC_TYPE Uart6;

/**************************************
�������� uint8_t USART6_RecService(void)
˵  ���� ����6���շ�������ж�������û�н�����
��  ���� ��
����ֵ�� 1���������		0������δ��ɻ�������
**************************************/
uint8_t USART6_RecService(void)		
{
	static uint16_t USART6_RecLen_Last=0;
	if(Uart6.RecCount)					//�������н���,���ٽ����һ�δ��ڽ����ж�
	{
//		#if Freertos_EN == 0		//�ȴ�1ms,������115200
//		HAL_Delay(2);
//		#endif
//		#if Freertos_EN == 1
//		osDelay(2);
//		#endif
		if(USART6_RecLen_Last == Uart6.RecCount)	//�жϽ������ݳ����Ƿ����ϴ�ѭ����ȣ������������������
		{
			Uart6.LastLen = Uart6.RecCount;					//��ȡ���ճ���
			Uart6.RecCount = 0;											//���´�Buff[0]��ʼ����
			for(uint16_t i=0;i<Uart6.LastLen;i++)  USART6_UserBuffer[i] = USART6_Buffer[i];		//�滻Buffer������Buffer��Ȼ��Ϊ�����ж���ʹ��			
			return 1;		
		}
		else USART6_RecLen_Last = Uart6.RecCount;
	}
	return 0;
}
#endif

/**************************************
�������� void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
˵  ���� ���ڽ����жϻص�����������Ҫ�����Լ�����
				 ���������ڽ����жϵ�ʱ��ϵͳ���Զ������������
				 ǰ����Ҫ�򿪽��գ�	HAL_UART_Receive_IT();
��  ���� *huart
����ֵ�� ��
**************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	#if USART1_EN
	if(huart == &huart1)		//����1�Ľ����ж�
	{
		//����������ݾͿ��������ﴦ����	
		USART1_Buffer[Uart1.RecCount] = Uart1.RecOneByte;
		Uart1.RecCount++;
		if(Uart1.RecCount >= USART1_RXBUF_LEN)  Uart1.RecCount=0;		//���ܴ��ڽ�������Ĵ�С
		HAL_UART_Receive_IT(&huart1,&Uart1.RecOneByte,1);						//���´򿪽���	
	}
	#endif
	
	#if USART2_EN
	if(huart == &huart2)		//����2�Ľ����ж�
	{
		//����������ݾͿ��������ﴦ����	
		USART2_Buffer[Uart2.RecCount] = Uart2.RecOneByte;
		Uart2.RecCount++;
		if(Uart2.RecCount >= USART2_RXBUF_LEN)  Uart2.RecCount=0;		//���ܴ��ڽ�������Ĵ�С
		HAL_UART_Receive_IT(&huart2,&Uart2.RecOneByte,1);						//���´򿪽���	
	}
	#endif
	
	#if USART3_EN
	if(huart == &huart3)		//����3�Ľ����ж�
	{
		//����������ݾͿ��������ﴦ����	
		USART3_Buffer[Uart3.RecCount] = Uart3.RecOneByte;
		Uart3.RecCount++;
		if(Uart3.RecCount >= USART3_RXBUF_LEN)  Uart3.RecCount=0;		//���ܴ��ڽ�������Ĵ�С
		HAL_UART_Receive_IT(&huart3,&Uart3.RecOneByte,1);						//���´򿪽���	
	}
	#endif
	
	#if USART4_EN
	if(huart == &huart4)		//����4�Ľ����ж�
	{
		//����������ݾͿ��������ﴦ����	
		USART4_Buffer[Uart4.RecCount] = Uart4.RecOneByte;
		Uart4.RecCount++;
		if(Uart4.RecCount >= USART4_RXBUF_LEN)	Uart4.RecCount=0;		//���ܴ��ڽ�������Ĵ�С
		HAL_UART_Receive_IT(&huart4,&Uart4.RecOneByte,1);						//���´򿪽���	
	}
	#endif
	
	#if USART5_EN
	if(huart == &huart5)		//����5�Ľ����ж�
	{
		//����������ݾͿ��������ﴦ����	
		USART5_Buffer[Uart5.RecCount] = Uart5.RecOneByte;
		Uart5.RecCount++;
		if(Uart5.RecCount >= USART5_RXBUF_LEN)	Uart5.RecCount=0;		//���ܴ��ڽ�������Ĵ�С
		HAL_UART_Receive_IT(&huart5,&Uart5.RecOneByte,1);						//���´򿪽���	
	}
	#endif
	
	#if USART6_EN
	if(huart == &huart6)		//����6�Ľ����ж�
	{
		//����������ݾͿ��������ﴦ����	
		USART6_Buffer[Uart6.RecCount] = Uart6.RecOneByte;
		Uart6.RecCount++;
		if(Uart6.RecCount >= USART6_RXBUF_LEN)	Uart6.RecCount=0;		//���ܴ��ڽ�������Ĵ�С
		HAL_UART_Receive_IT(&huart6,&Uart6.RecOneByte,1);						//���´򿪽���	
	}
	#endif
}
