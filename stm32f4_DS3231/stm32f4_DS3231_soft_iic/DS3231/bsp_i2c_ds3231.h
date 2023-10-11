/*
******************************************************************************
* @file    bsp_i2c_ds3231.h
* @author  �̛�
* @version V1.0
* @date    2020-11-18
* @brief   ģ��i2c RTC(DS3231)Ӧ�ú���bsp
******************************************************************************
* @attention
*
******************************************************************************
*/


/******************************************************************************
* @attention
*
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_DS3231_H
#define	__I2C_DS3231_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint8_t  hour;
    uint8_t  min;
    uint8_t  sec;
    uint32_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  week;
    uint16_t temperature;
} _calendar_obj;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/**************************I2C�������壬I2C1��I2C2*****************************/




//#define	DS3231_I2C_GPIO_APBxClock_FUN   RCC_AHB1PeriphClockCmd
//#define   DS3231_I2C_GPIO_CLK             RCC_APB2Periph_GPIOC
#define     DS3231_I2C_SCL_PORT             GPIOC
#define     DS3231_I2C_SCL_PIN              GPIO_PIN_14
#define     DS3231_I2C_SDA_PORT             GPIOC
#define			DS3231_I2C_SDA_PIN							GPIO_PIN_13



#define 	I2C_WR		0
#define 	I2C_RD		1


#define		I2C_SCL_1()  			HAL_GPIO_WritePin(DS3231_I2C_SCL_PORT, DS3231_I2C_SCL_PIN,GPIO_PIN_SET)
#define 	I2C_SCL_0()  			HAL_GPIO_WritePin(DS3231_I2C_SCL_PORT, DS3231_I2C_SCL_PIN,GPIO_PIN_RESET)
#define 	I2C_SDA_1()           	HAL_GPIO_WritePin(DS3231_I2C_SDA_PORT, DS3231_I2C_SDA_PIN,GPIO_PIN_SET)
#define 	I2C_SDA_0()           	HAL_GPIO_WritePin(DS3231_I2C_SDA_PORT, DS3231_I2C_SDA_PIN,GPIO_PIN_RESET)
#define 	I2C_SDA_READ()        	HAL_GPIO_ReadPin(DS3231_I2C_SDA_PORT, DS3231_I2C_SDA_PIN)


/* STM32 I2C ����ģʽ */
//#define             I2C_Speed             										 400000
/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
//#define 						I2Cx_OWN_ADDRESS7     										 0X0A
/* DS3231 ��ַ���� */
#define 						DS3231_ADDRESS 														 0xD0

/* DS3231�Ĵ�����ַ */

#define							DS3231_SECOND															 0x00    //�� 0-59
#define 						DS3231_MINUTE      												 0x01    //�� 0-59
#define 						DS3231_HOUR        												 0x02    //ʱ 1-12AM/PM 0-23
#define 						DS3231_WEEK         											 0x03    //���� 1-7
#define 						DS3231_DAY          											 0x04    //�� 1-31
#define 						DS3231_MONTH                      		   	 0x05    //�� 1-12
#define             DS3231_YEAR        						   					 0x06    //�� 0-99											 
/* ����1 */
#define             DS3231_SALARM1ECOND                  			 0x07    //��
#define 						DS3231_ALARM1MINUTE                        0x08    //��
#define             DS3231_ALARM1HOUR                          0x09    //ʱ
#define 						DS3231_ALARM1WEEK  												 0x0A    //����/��
/* ����2 */
#define 						DS3231_ALARM2MINUTE 											 0x0b    //��
#define 						DS3231_ALARM2HOUR                          0x0c    //ʱ
#define 						DS3231_ALARM2WEEK                          0x0d    //����/��

#define 						DS3231_CONTROL                             0x0e    //���ƼĴ���
#define 						DS3231_STATUS                              0x0f    //״̬�Ĵ���
#define 						BSY                 											 2       //æ
#define 						OSF                												 7       //����ֹͣ��־
#define 						DS3231_XTAL         											 0x10    //�����ϻ��Ĵ���
#define 						DS3231_TEMPERATUREH 											 0x11    //�¶ȼĴ������ֽ�(8λ)
#define 						DS3231_TEMPERATUREL 											 0x12    //�¶ȼĴ������ֽ�(��2λ) 																				



/* Exported functions ------------------------------------------------------- */
void I2C_DS3231_Init(void);
void i2c_Stop(void);
uint32_t I2C_DS3231_ByteWrite(uint8_t WriteAddr, uint8_t data);
uint8_t I2C_DS3231_DataRead(uint8_t ReadAddr);
uint8_t BCD_DEC(uint8_t val);
uint8_t DEC_BCD(uint8_t val);
void i2c_SendByte(uint8_t _ucByte);
void I2C_DS3231_SetTime(uint8_t yea,uint8_t mon,uint8_t da,uint8_t we,uint8_t hou,uint8_t min,uint8_t sec);
void Time_Regulate_Get(_calendar_obj *tm);

void I2C_DS3231_getTime(void);
void I2C_DS3231_getTemperature(void);



#endif
/*********************************************END OF FILE**********************/
