#include "bsp_i2c_ds3231.h"
#include "usart.h"
#include "stdio.h"
/**
  ******************************************************************************
  * @file    bsp_i2c_ds3231.c
  * @author  �̛�
  * @version V1.0
  * @date    2020-11-16
  * @brief   i2c RTC(DS3231)Ӧ�ú���bsp
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


_calendar_obj calendar;	//�����ṹ��


/**
 * @brief  ת����BCD��
 * @param  None
 * @retval ����BCD��
 */
uint16_t B_BCD(uint8_t val)
{
    uint8_t i,j,k;
    i=val/10;
    j=val%10;
    k=j+(i<<4);
    return k;
}

/**
  * @brief  I2C I/O����
  * @param  None
  * @retval None
  */
static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    /* ʹ���� I2C �йص�ʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE();           	// ʹ��GPIOC�˿�ʱ��

    /* I2C_SCL��I2C_SDA*/
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  	//��©���
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//����
    GPIO_Initure.Pin=DS3231_I2C_SCL_PIN|DS3231_I2C_SDA_PIN; 						//PC3
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    HAL_GPIO_WritePin(GPIOC,DS3231_I2C_SCL_PIN|DS3231_I2C_SDA_PIN,GPIO_PIN_SET);	//��ʼ������1
}

/**
  * @brief  I2C ����(DS3231)��ʼ��
  * @param  None
  * @retval None
  */
void I2C_DS3231_Init(void)
{
    I2C_GPIO_Config();

    /* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
    i2c_Stop();
}

/**
  * @brief  I2C����λ�ӳ٣����400KHz
  * @param  None
  * @retval None
  */
static void i2c_Delay(void)
{
    uint8_t i;

    /*��
     	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�

    	ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz
    	ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us
     	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us
    */
    for (i = 0; i < 20; i++);
}

/**
  * @brief  I2C���������ź�
  * @param  None
  * @retval None
  */
void i2c_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    I2C_SDA_1();
    I2C_SCL_1();
    i2c_Delay();
    I2C_SDA_0();
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
}

/**
  * @brief  I2C����ֹͣ�ź�
  * @param  None
  * @retval None
  */
void i2c_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    I2C_SDA_0();
    I2C_SCL_1();
    i2c_Delay();
    I2C_SDA_1();
}

/**
  * @brief  MCU��I2C�����豸����8bit����
  * @param
  *		@arg 	_ucByte:���͵��ֽ�
	* @retval None
  */
void i2c_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            I2C_SDA_1();
        }
        else
        {
            I2C_SDA_0();
        }
        i2c_Delay();
        I2C_SCL_1();
        i2c_Delay();
        I2C_SCL_0();
        if (i == 7)
        {
            I2C_SDA_1(); // �ͷ�����
        }
        _ucByte <<= 1;	/* ����һ��bit */
        i2c_Delay();
    }
}

/**
  * @brief  MCU��I2C�����豸��ȡ8bit����
  * @param  None
	* @retval ����������
  */
uint8_t i2c_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        I2C_SCL_1();
        i2c_Delay();
        if (I2C_SDA_READ())
        {
            value++;
        }
        I2C_SCL_0();
        i2c_Delay();
    }
    return value;
}

/**
  * @brief  MCU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * @param  None
	* @retval ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  */
uint8_t i2c_WaitAck(void)
{
    uint8_t re;

    I2C_SDA_1();	/* CPU�ͷ�SDA���� */
    i2c_Delay();
    I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    i2c_Delay();
    if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    I2C_SCL_0();
    i2c_Delay();
    return re;
}

/**
  * @brief  MCU����һ��ACK�ź�
  * @param  None
	* @retval None
  */
void i2c_Ack(void)
{
    I2C_SDA_0();	/* CPU����SDA = 0 */
    i2c_Delay();
    I2C_SCL_1();	/* CPU����1��ʱ�� */
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
    I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/**
  * @brief  MCU����1��NACK�ź�
  * @param  None
	* @retval None
  */
void i2c_NAck(void)
{
    I2C_SDA_1();	/* CPU����SDA = 1 */
    i2c_Delay();
    I2C_SCL_1();	/* CPU����1��ʱ�� */
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
}

/**
  * @brief  ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
  * @param
  *		@arg  _Address:�豸��I2C���ߵ�ַ
	* @retval ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
  */
uint8_t i2c_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    I2C_DS3231_Init();		/* ����GPIO */


    i2c_Start();		/* ���������ź� */

    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    i2c_SendByte(_Address | I2C_WR);
    ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

    i2c_Stop();			/* ����ֹͣ�ź� */

    return ucAck;
}

//
/**
  * @brief	дһ���ֽڵ�I2C DS3231��
  * @param
  *		@arg data:Ҫд����ֽ�
  *		@arg WriteAddr:д��ַ
	* @retval ����1����ʾд��ɹ�.
  */
uint32_t I2C_DS3231_ByteWrite(uint8_t WriteAddr, uint8_t data)
{
    /* ��1��������I2C���������ź� */
    i2c_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    i2c_SendByte(DS3231_ADDRESS| I2C_WR);	/* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��4�������ͼĴ�����ַ */
    i2c_SendByte((uint8_t)WriteAddr);

    /* ��5�����ȴ�ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��6������ʼд������ */
    i2c_SendByte(data);

    /* ��7�����ȴ�ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    i2c_Stop();
    return 1;
cmd_fail: /* ����ִ��ʧ�ܺ󣬷���ֹͣ�ź� */
    /* ����I2C����ֹͣ�ź� */
    i2c_Stop();
    return 0;
}

/**
  * @brief	��DS3231�����ȡһ���ֽ�����
  * @param
  *		@arg data:��Ŵ�DS3231��ȡ������
  *		@arg ReadAddr:��ȡ���ݵ�DS3231�ĵ�ַ
	* @retval data:��������.
  */
uint8_t I2C_DS3231_DataRead(uint8_t ReadAddr)
{
    uint8_t data;

    /* ��1��������I2C���������ź� */
    i2c_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    i2c_SendByte(DS3231_ADDRESS|I2C_WR);	/* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��4��������DS3231�Ĵ�����ַ */
    i2c_SendByte((uint8_t)ReadAddr);

    /* ��5�����ȴ�ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��6���������ڶ��� I2C ��ʼ�ź� */
    i2c_Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    //i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* �˴��Ƕ�ָ�� */
    i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��9������ȡ���� */
    data = i2c_ReadByte();

    i2c_NAck();
    i2c_Stop();
    return data;

cmd_fail: /* ����ִ��ʧ�ܺ󣬷���ֹͣ�ź� */
    /* ����I2C����ֹͣ�ź� */
    i2c_Stop();
    return 0;
}

/**
  * @brief	BCD(8421)תDEC.
  * @param  val��BCD��.
  * @retval i��DEC��.
  */
uint8_t BCD_DEC(uint8_t val)
{
    uint8_t i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    return i;
}

/**
  * @brief	DECתBCD(8421).
  * @param  val��DEC��.
  * @retval k��BCD��.
  */
uint8_t DEC_BCD(uint8_t val)
{
    uint8_t i,j,k;
    i=val/10;
    j=val%10;
    k=j+(i<<4);
    return k;
}

/**
  * @brief	ʱ������
  * @param
  *		@arg 	�ֱ����� �� �� �� ���� ʱ �� ��
  * @retval ��
  */
void I2C_DS3231_SetTime(uint8_t yea,uint8_t mon,uint8_t da,uint8_t we,uint8_t hou,uint8_t min,uint8_t sec)
{
    uint8_t temp=0;
    yea-=2000;
    temp=DEC_BCD(yea);
    I2C_DS3231_ByteWrite(0x06,temp);
	
    temp=DEC_BCD(mon);
    I2C_DS3231_ByteWrite(0x05,temp);

    temp=DEC_BCD(da);
    I2C_DS3231_ByteWrite(0x04,temp);

		temp=DEC_BCD(we);
		I2C_DS3231_ByteWrite(0x03,temp);

    temp=DEC_BCD(hou);
    I2C_DS3231_ByteWrite(0x02,temp);

    temp=DEC_BCD(min);
    I2C_DS3231_ByteWrite(0x01,temp);

    temp=DEC_BCD(sec);
    I2C_DS3231_ByteWrite(0x00,temp);
}

/**
  * @brief	��ȡʱ��
  * @param
  *		@arg pBuffer:��Ŵ�DS3231��ȡ�����ݵĻ�����ָ��
  *		@arg ReadAddr:��ȡ���ݵ�DS3231�ĵ�ַ
  *   @arg NumByteToWrite:Ҫ��DS3231��ȡ���ֽ���
  * @retval ����1����ʾ��ȡ�ɹ�.
  */
void I2C_DS3231_getTime(void)
{
    calendar.year=I2C_DS3231_DataRead(0x06);
    calendar.year=BCD_DEC(calendar.year)+2000;//yearֻ������0-99������2000������ʾ���ڵ�ʱ��

    calendar.month=I2C_DS3231_DataRead(0x05);
    calendar.month=BCD_DEC(calendar.month);

    calendar.day=I2C_DS3231_DataRead(0x04);
    calendar.day=BCD_DEC(calendar.day);

    calendar.week=I2C_DS3231_DataRead(0x03);
    calendar.week=BCD_DEC(calendar.week);

    calendar.hour=I2C_DS3231_DataRead(0x02);
    calendar.hour&=0x3f;
    calendar.hour=BCD_DEC(calendar.hour);

    calendar.min=I2C_DS3231_DataRead(0x01);
    calendar.min=BCD_DEC(calendar.min);


    calendar.sec=I2C_DS3231_DataRead(0x00);
    calendar.sec=BCD_DEC(calendar.sec);
}

/**
  * @brief �����û�ʹ�ô������õ�ʱ��
  * @param
  *		@arg tm:��������RTCʱ��Ľṹ��ָ��
  * @retval
  */
void Time_Regulate_Get(_calendar_obj *tm)
{
		uint8_t temp = 0;
	  uint32_t temp_num = 0;
		uint8_t day_max=0 ;

	  printf("\r\n=========================����ʱ��==================");

	  do
	  {
			printf("\r\n  ���������(Please Set Years),��Χ[2000~2255]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);
			if(temp_num <2000 || temp_num >2255)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);

			}
			else
			{
				printf("\n\r  ��ݱ�����Ϊ: %d\n\r", temp_num);
        temp_num-=2000;
				temp = DEC_BCD(temp_num);
				tm->year = temp_num;
				I2C_DS3231_ByteWrite(0x06,temp);
				break;
			}
	  }while(1);


	 do
	  {
			printf("\r\n  �������·�(Please Set Months):��Χ[1~12]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);
			if(temp_num <1 || temp_num >12)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);

			}
			else
			{
				printf("\n\r  �·ݱ�����Ϊ: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->month = temp_num;
				I2C_DS3231_ByteWrite(0x05,temp);
				break;
			}
	  }while(1);

		/*�����·ݼ����������*/
		switch(tm->month)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
						day_max = 31;
					break;

				case 4:
				case 6:
				case 9:
				case 11:
						day_max = 30;
					break;

				case 2:
				     /*��������*/
						if(((tm->year+2000)%4==0) &&
							 (((tm->year+2000)%100!=0) || ((tm->year+2000)%400==0)))
								{
									day_max = 29;
								} else
								{
									day_max = 28;
								}
					break;
			}

		do
	  {
			printf("\r\n  ����������(Please Set days),��Χ[1~%d]�������ַ�����ӻس�:",day_max);
			scanf("%d",&temp_num);

			if(temp_num <1 || temp_num >day_max)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ���ڱ�����Ϊ: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->day = temp_num;
				I2C_DS3231_ByteWrite(0x04,temp);
				break;
			}
	  }while(1);

		
		do
	  {
			printf("\r\n  ����������(Please Set week),��Χ[1~7]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);

			if(temp_num <1 || temp_num >7)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ���ڱ�����Ϊ: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->day = temp_num;
				I2C_DS3231_ByteWrite(0x03,temp);
				break;
			}
	  }while(1);


		do
	  {
			printf("\r\n  ������ʱ��(Please Set Hours),��Χ[0~23]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);

			if( temp_num >23)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ʱ�ӱ�����Ϊ: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->hour = temp_num;
				I2C_DS3231_ByteWrite(0x02,temp);
				break;
			}
	  }while(1);

		do
	  {
			printf("\r\n  ���������(Please Set Minutes),��Χ[0~59]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);

			if( temp_num >59)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ���ӱ�����Ϊ: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->min = temp_num;
				I2C_DS3231_ByteWrite(0x01,temp);
				break;
			}
	  }while(1);

		do
	  {
			printf("\r\n  ����������(Please Set Seconds),��Χ[0~59]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);

			if( temp_num >59)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ���ӱ�����Ϊ: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->sec = temp_num;
				I2C_DS3231_ByteWrite(0x00,temp);
				break;
			}
	  }while(1);
		__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);	//���������ж�
}


/**
  * @brief	��ȡ�¶�
  * @param  ��
  * @retval ��
  */
void I2C_DS3231_getTemperature(void)
{
    //I2C_DS3231_ByteWrite(DS3231_CONTROL, 0x20|0x05);
    calendar.temperature=I2C_DS3231_DataRead(DS3231_TEMPERATUREH);
}


/*********************************************END OF FILE**********************/
