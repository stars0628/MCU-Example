#ifndef __MY_USART_H_
#define __MY_USART_H_	



//�ر�ע�⣺
//1.CubeMX����ʱҪ�򿪴����ж�
//2.����ѭ����Ҫ����	HAL_UART_Receive_IT(&huartx,&Uartx.RecOneByte,1)	���ܽ����ж�	
//3.�����ʵ���9600ʱ������ú����ļ��̫��/С��1ms������Ҫ���������
//4.CubeMX����ʱ��NVIC - select for init sequence ordering Ҫ��

//API: 
//HAL_UART_Receive_IT(&huartx,&Uartx.RecOneByte,1);		//����һ�ν��գ���Ҫ���ڴ��ڳ�ʼ����ѭ����ʼǰ
//uint8_t USART1_RecService(void);										//���ڷ����������ڲ�ѯ���ڽ����Ƿ���ɣ�Ӧ�÷���ѭ����ѯ���Ҳ�ѯ�������С��1ms(���ر�ע�������)	
//�û���ʹ�õĲ�����		Uartx.LastLen: �������ݵĳ���		USART1_UserBuffer[]: ��Ž������ݵ�����

#define Freertos_EN			0			//ʹ��FreeRTOS��1		��ʹ�ã�0  
#define USART1_EN				1			//ʹ�ô��ڣ�1				��ʹ�ã�0
#define USART2_EN				0			//ʹ�ô��ڣ�1				��ʹ�ã�0
#define USART3_EN				0			//ʹ�ô��ڣ�1				��ʹ�ã�0
#define USART4_EN				0			//ʹ�ô��ڣ�1				��ʹ�ã�0
#define USART5_EN				0			//ʹ�ô��ڣ�1				��ʹ�ã�0
#define USART6_EN				0			//ʹ�ô��ڣ�1				��ʹ�ã�0
		
#define USART1_RXBUF_LEN 100	//���崮�ڽ���Buffer����
#define USART2_RXBUF_LEN 100
#define USART3_RXBUF_LEN 50
#define USART4_RXBUF_LEN 50
#define USART5_RXBUF_LEN 100
#define USART6_RXBUF_LEN 100

extern unsigned char USART1_Buffer[USART1_RXBUF_LEN];
extern unsigned char USART1_UserBuffer[USART1_RXBUF_LEN];

extern unsigned char USART2_UserBuffer[USART2_RXBUF_LEN];
extern unsigned char USART2_Buffer[USART2_RXBUF_LEN];

extern unsigned char USART3_Buffer[USART3_RXBUF_LEN];
extern unsigned char USART3_UserBuffer[USART3_RXBUF_LEN];

extern unsigned char USART4_Buffer[USART4_RXBUF_LEN];
extern unsigned char USART4_UserBuffer[USART4_RXBUF_LEN];

extern unsigned char USART5_Buffer[USART5_RXBUF_LEN];
extern unsigned char USART5_UserBuffer[USART5_RXBUF_LEN];

extern unsigned char USART6_Buffer[USART6_RXBUF_LEN];
extern unsigned char USART6_UserBuffer[USART6_RXBUF_LEN];

unsigned char USART1_RecService(void);
unsigned char USART2_RecService(void);
unsigned char USART3_RecService(void);	
unsigned char USART4_RecService(void);		
unsigned char USART5_RecService(void);
unsigned char USART6_RecService(void);

struct UART_REC_TYPE
{
	unsigned char  RecOneByte;		//�����жϽ���
	unsigned short int RecCount;	//���ݳ���
	unsigned short int LastLen;		//��һ�ε����ݳ��������жϽ������
};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

extern struct UART_REC_TYPE Uart1,Uart2,Uart3,Uart4,Uart5,Uart6;

#endif
