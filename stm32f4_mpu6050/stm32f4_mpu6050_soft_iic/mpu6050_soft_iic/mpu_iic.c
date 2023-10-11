#include "mpu_iic.h"
//#include "usart.h"

/*
IIC�ӿ���������
SDA:PB11
SCL:PB10
AD0:PB2
*/
void MPU_IIC_Init( void )
{
	MPU_IIC_SDA_1();
	MPU_IIC_SCL_1();

}


void MPU_IIC_Start( void )
{
	MPU_SDA_OUT();
	
	MPU_IIC_SDA_1();
	MPU_IIC_SCL_1();
	delay_us(30);
	MPU_IIC_SDA_0();
	delay_us(30);
	MPU_IIC_SCL_0();
}

void MPU_IIC_Stop( void )
{
	MPU_SDA_OUT();
	
	MPU_IIC_SDA_0();
	MPU_IIC_SCL_1();
	delay_us(30);
	MPU_IIC_SDA_1();
	MPU_IIC_SCL_1();
	delay_us(30);
	
}

/* �ɴ��豸��SCLΪ�ߵ�ƽ��ʱ������SDA��ΪӦ�� 
����ֵ��1��δӦ��
        0����Ӧ��
*/
uint8_t MPU_IIC_Wait_Ack( void )
{
	uint8_t count;
	MPU_SDA_IN();
	
	MPU_IIC_SCL_1();
	delay_us(30);
	MPU_IIC_SDA_1();
	delay_us(30);
	
	while( MPU_IIC_SDA_READ()==1 )
	{
		count++;
		if( count>250 )
		{
			MPU_IIC_Stop();
			return 1;
		}
	}	
	MPU_IIC_SCL_0();
	return 0;
}


void MPU_IIC_Ack( void )
{
	
	MPU_IIC_SCL_0();
	MPU_SDA_OUT();
	MPU_IIC_SDA_0();
	delay_us(30);
	MPU_IIC_SCL_1();
	delay_us(30);
	MPU_IIC_SCL_0();
	
}


void MPU_IIC_NAck( void )
{
	
	
	MPU_IIC_SCL_0();
	MPU_SDA_OUT();
	MPU_IIC_SDA_1();
	delay_us(30);
	MPU_IIC_SCL_1();
	delay_us(30);
	MPU_IIC_SCL_0();
}


/* ����һ���ֽ����ݣ���λ���� */
void MPU_IIC_Send_Byte( uint8_t data )
{
	uint8_t t;
	MPU_SDA_OUT();
	
	MPU_IIC_SCL_0();
	for( t=0;t<8;t++ )
	{
		if( ((data&0x80)>>7)==1 )
			MPU_IIC_SDA_1();
		else
			MPU_IIC_SDA_0();
		data<<=1;
		MPU_IIC_SCL_1();
		delay_us(30);
		MPU_IIC_SCL_0();
		delay_us(30);
	}
}


/* ��ȡһ���ֽڣ�ack=1ʱ����ȡ��ɺ���������Ӧ�� */
uint8_t MPU_IIC_Read_Byte( uint8_t ack )
{
	uint8_t t,data=0;
	MPU_SDA_IN();
	for( t=0;t<8;t++ )
	{
		MPU_IIC_SCL_0();
		delay_us(30);//�ȴ�SDA�ı仯
		MPU_IIC_SCL_1();
		
		data<<=1;//�����ڶ�ȡǰ�棬��Ϊ֮��һλ��ȡ��Ͳ�����λ
		if( MPU_IIC_SDA_READ()==1 )
			data++;
		
		delay_us(30);//�ȴ�SDA�ı仯
		
	}

	if( !ack )
		MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK 
	return data;
}

void MPU_SDA_IN()//����SDAΪ����
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
	MPU6050_GPIO_CLK_ENABLE();
  /*Configure GPIO pin Output Level */

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MPU6050_GPIO, MPU6050_SDA,GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = MPU6050_SDA;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MPU6050_GPIO, &GPIO_InitStruct);
}

void MPU_SDA_OUT()//����SDAΪ���
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  MPU6050_GPIO_CLK_ENABLE();
  /*Configure GPIO pin Output Level */

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MPU6050_GPIO, MPU6050_SDA,GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = MPU6050_SDA;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MPU6050_GPIO, &GPIO_InitStruct);
}

void delay_us(uint16_t us)//΢�뼶��ʱ��ѭ����
{
	uint8_t system_clock = clock;//ʱ��Ƶ��
	uint16_t nus = us * system_clock;
	do
	 {
		 __NOP();
	 }
	 while(nus--);
}
