#include "bsp_i2c_ds3231.h"
#include "usart.h"
#include "stdio.h"
/**
  ******************************************************************************
  * @file    bsp_i2c_ds3231.c
  * @author  兲涳
  * @version V1.0
  * @date    2020-11-16
  * @brief   i2c RTC(DS3231)应用函数bsp
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


_calendar_obj calendar;	//日历结构体


/**
 * @brief  转换成BCD码
 * @param  None
 * @retval 返回BCD码
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
  * @brief  I2C I/O配置
  * @param  None
  * @retval None
  */
static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    /* 使能与 I2C 有关的时钟 */
    __HAL_RCC_GPIOC_CLK_ENABLE();           	// 使能GPIOC端口时钟

    /* I2C_SCL、I2C_SDA*/
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  	//开漏输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
    GPIO_Initure.Pin=DS3231_I2C_SCL_PIN|DS3231_I2C_SDA_PIN; 						//PC3
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    HAL_GPIO_WritePin(GPIOC,DS3231_I2C_SCL_PIN|DS3231_I2C_SDA_PIN,GPIO_PIN_SET);	//初始化后置1
}

/**
  * @brief  I2C 外设(DS3231)初始化
  * @param  None
  * @retval None
  */
void I2C_DS3231_Init(void)
{
    I2C_GPIO_Config();

    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    i2c_Stop();
}

/**
  * @brief  I2C总线位延迟，最快400KHz
  * @param  None
  * @retval None
  */
static void i2c_Delay(void)
{
    uint8_t i;

    /*　
     	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化

    	循环次数为10时，SCL频率 = 205KHz
    	循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
     	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us
    */
    for (i = 0; i < 20; i++);
}

/**
  * @brief  I2C总线启动信号
  * @param  None
  * @retval None
  */
void i2c_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    I2C_SDA_1();
    I2C_SCL_1();
    i2c_Delay();
    I2C_SDA_0();
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
}

/**
  * @brief  I2C总线停止信号
  * @param  None
  * @retval None
  */
void i2c_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    I2C_SDA_0();
    I2C_SCL_1();
    i2c_Delay();
    I2C_SDA_1();
}

/**
  * @brief  MCU向I2C总线设备发送8bit数据
  * @param
  *		@arg 	_ucByte:发送的字节
	* @retval None
  */
void i2c_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
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
            I2C_SDA_1(); // 释放总线
        }
        _ucByte <<= 1;	/* 左移一个bit */
        i2c_Delay();
    }
}

/**
  * @brief  MCU从I2C总线设备读取8bit数据
  * @param  None
	* @retval 读到的数据
  */
uint8_t i2c_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
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
  * @brief  MCU产生一个时钟，并读取器件的ACK应答信号
  * @param  None
	* @retval 返回0表示正确应答，1表示无器件响应
  */
uint8_t i2c_WaitAck(void)
{
    uint8_t re;

    I2C_SDA_1();	/* CPU释放SDA总线 */
    i2c_Delay();
    I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    i2c_Delay();
    if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
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
  * @brief  MCU产生一个ACK信号
  * @param  None
	* @retval None
  */
void i2c_Ack(void)
{
    I2C_SDA_0();	/* CPU驱动SDA = 0 */
    i2c_Delay();
    I2C_SCL_1();	/* CPU产生1个时钟 */
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
    I2C_SDA_1();	/* CPU释放SDA总线 */
}

/**
  * @brief  MCU产生1个NACK信号
  * @param  None
	* @retval None
  */
void i2c_NAck(void)
{
    I2C_SDA_1();	/* CPU驱动SDA = 1 */
    i2c_Delay();
    I2C_SCL_1();	/* CPU产生1个时钟 */
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
}

/**
  * @brief  检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
  * @param
  *		@arg  _Address:设备的I2C总线地址
	* @retval 返回值 0 表示正确， 返回1表示未探测到
  */
uint8_t i2c_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    I2C_DS3231_Init();		/* 配置GPIO */


    i2c_Start();		/* 发送启动信号 */

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    i2c_SendByte(_Address | I2C_WR);
    ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

    i2c_Stop();			/* 发送停止信号 */

    return ucAck;
}

//
/**
  * @brief	写一个字节到I2C DS3231中
  * @param
  *		@arg data:要写入的字节
  *		@arg WriteAddr:写地址
	* @retval 返回1，表示写入成功.
  */
uint32_t I2C_DS3231_ByteWrite(uint8_t WriteAddr, uint8_t data)
{
    /* 第1步：发起I2C总线启动信号 */
    i2c_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    i2c_SendByte(DS3231_ADDRESS| I2C_WR);	/* 此处是写指令 */

    /* 第3步：等待ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送寄存器地址 */
    i2c_SendByte((uint8_t)WriteAddr);

    /* 第5步：等待ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第6步：开始写入数据 */
    i2c_SendByte(data);

    /* 第7步：等待ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    i2c_Stop();
    return 1;
cmd_fail: /* 命令执行失败后，发送停止信号 */
    /* 发送I2C总线停止信号 */
    i2c_Stop();
    return 0;
}

/**
  * @brief	从DS3231里面读取一个字节数据
  * @param
  *		@arg data:存放从DS3231读取的数据
  *		@arg ReadAddr:读取数据的DS3231的地址
	* @retval data:返回数据.
  */
uint8_t I2C_DS3231_DataRead(uint8_t ReadAddr)
{
    uint8_t data;

    /* 第1步：发起I2C总线启动信号 */
    i2c_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    i2c_SendByte(DS3231_ADDRESS|I2C_WR);	/* 此处是写指令 */

    /* 第3步：等待ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送DS3231寄存器地址 */
    i2c_SendByte((uint8_t)ReadAddr);

    /* 第5步：等待ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第6步：产生第二次 I2C 起始信号 */
    i2c_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    //i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* 此处是读指令 */
    i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* 此处是读指令 */

    /* 第8步：发送ACK */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第9步：读取数据 */
    data = i2c_ReadByte();

    i2c_NAck();
    i2c_Stop();
    return data;

cmd_fail: /* 命令执行失败后，发送停止信号 */
    /* 发送I2C总线停止信号 */
    i2c_Stop();
    return 0;
}

/**
  * @brief	BCD(8421)转DEC.
  * @param  val：BCD码.
  * @retval i：DEC码.
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
  * @brief	DEC转BCD(8421).
  * @param  val：DEC码.
  * @retval k：BCD码.
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
  * @brief	时间设置
  * @param
  *		@arg 	分别输入 年 月 日 星期 时 分 秒
  * @retval 无
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
  * @brief	获取时间
  * @param
  *		@arg pBuffer:存放从DS3231读取的数据的缓冲区指针
  *		@arg ReadAddr:读取数据的DS3231的地址
  *   @arg NumByteToWrite:要从DS3231读取的字节数
  * @retval 返回1，表示读取成功.
  */
void I2C_DS3231_getTime(void)
{
    calendar.year=I2C_DS3231_DataRead(0x06);
    calendar.year=BCD_DEC(calendar.year)+2000;//year只能设置0-99，加上2000用来表示现在的时间

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
  * @brief 保存用户使用串口设置的时间
  * @param
  *		@arg tm:用于设置RTC时间的结构体指针
  * @retval
  */
void Time_Regulate_Get(_calendar_obj *tm)
{
		uint8_t temp = 0;
	  uint32_t temp_num = 0;
		uint8_t day_max=0 ;

	  printf("\r\n=========================设置时间==================");

	  do
	  {
			printf("\r\n  请输入年份(Please Set Years),范围[2000~2255]，输入字符后请加回车:");
			scanf("%d",&temp_num);
			if(temp_num <2000 || temp_num >2255)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);

			}
			else
			{
				printf("\n\r  年份被设置为: %d\n\r", temp_num);
        temp_num-=2000;
				temp = DEC_BCD(temp_num);
				tm->year = temp_num;
				I2C_DS3231_ByteWrite(0x06,temp);
				break;
			}
	  }while(1);


	 do
	  {
			printf("\r\n  请输入月份(Please Set Months):范围[1~12]，输入字符后请加回车:");
			scanf("%d",&temp_num);
			if(temp_num <1 || temp_num >12)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);

			}
			else
			{
				printf("\n\r  月份被设置为: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->month = temp_num;
				I2C_DS3231_ByteWrite(0x05,temp);
				break;
			}
	  }while(1);

		/*根据月份计算最大日期*/
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
				     /*计算闰年*/
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
			printf("\r\n  请输入日期(Please Set days),范围[1~%d]，输入字符后请加回车:",day_max);
			scanf("%d",&temp_num);

			if(temp_num <1 || temp_num >day_max)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  日期被设置为: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->day = temp_num;
				I2C_DS3231_ByteWrite(0x04,temp);
				break;
			}
	  }while(1);

		
		do
	  {
			printf("\r\n  请输入星期(Please Set week),范围[1~7]，输入字符后请加回车:");
			scanf("%d",&temp_num);

			if(temp_num <1 || temp_num >7)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  日期被设置为: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->day = temp_num;
				I2C_DS3231_ByteWrite(0x03,temp);
				break;
			}
	  }while(1);


		do
	  {
			printf("\r\n  请输入时钟(Please Set Hours),范围[0~23]，输入字符后请加回车:");
			scanf("%d",&temp_num);

			if( temp_num >23)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  时钟被设置为: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->hour = temp_num;
				I2C_DS3231_ByteWrite(0x02,temp);
				break;
			}
	  }while(1);

		do
	  {
			printf("\r\n  请输入分钟(Please Set Minutes),范围[0~59]，输入字符后请加回车:");
			scanf("%d",&temp_num);

			if( temp_num >59)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  分钟被设置为: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->min = temp_num;
				I2C_DS3231_ByteWrite(0x01,temp);
				break;
			}
	  }while(1);

		do
	  {
			printf("\r\n  请输入秒钟(Please Set Seconds),范围[0~59]，输入字符后请加回车:");
			scanf("%d",&temp_num);

			if( temp_num >59)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  秒钟被设置为: %d\n\r", temp_num);
        temp = DEC_BCD(temp_num);
				tm->sec = temp_num;
				I2C_DS3231_ByteWrite(0x00,temp);
				break;
			}
	  }while(1);
		__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);	//开启接收中断
}


/**
  * @brief	获取温度
  * @param  无
  * @retval 无
  */
void I2C_DS3231_getTemperature(void)
{
    //I2C_DS3231_ByteWrite(DS3231_CONTROL, 0x20|0x05);
    calendar.temperature=I2C_DS3231_DataRead(DS3231_TEMPERATUREH);
}


/*********************************************END OF FILE**********************/
