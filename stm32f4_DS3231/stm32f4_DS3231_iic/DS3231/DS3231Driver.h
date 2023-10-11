/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : DS3231.h
  * @brief          : Header for DS3231.c file.
  *                   This file provides code for the configuration
  *                   of the DS3231 instances
  * @author         : Lesterbor
  *	@time			:	2021-09-26
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DS3231DRIVER_H_
#define __DS3231DRIVER_H_
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stm32f4xx_hal.h"
	
/* USER CODE END Includes */
	
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PT */



#define 	I2C_WR					0x00		//дָ��
#define 	I2C_RD					0x01		//��ָ��
#define 	DS3231_ADDRESS 			0xD0   		//DS3231��ַ

#define 	Seconds_Register      	0x00     	//���ӼĴ���
#define 	Minutes_Register		0x01     	//���ӼĴ���
#define 	Hour_Register  			0x02     	//Сʱ�Ĵ���
#define 	Day_Register       		0x03     	//���ڼĴ���
#define 	Date_Register      		0x04		//���ڼĴ���
#define	 	Month_Register     		0x05    	//�·ݼĴ���������λ������λ��
#define 	Year_Register       	0x06    	//��ݼĴ���
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
/* USER CODE END PT */
	
/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

uint8_t BCD_DEC(uint8_t BCD_Data);
uint8_t DEC_BCD(uint8_t DEC_Data);
uint8_t I2C_DS3231_ReadData(uint8_t ReadAddr);
uint8_t I2C_DS3231_WriteData(uint8_t WriteAddr,uint8_t Data);
void DS3231_SetTime(uint8_t Yea,uint8_t Mon,uint8_t Dat,uint8_t Wee,uint8_t Hou,uint8_t Min,uint8_t Sec);
void DS3231_ReadTime(void);
uint8_t *DS3231_ReadTime_ReturnPoint(void);
void DS3231_Temp(void);
/* USER CODE END EFP */

#endif /* __DS3231_H_ */
/************************ (C) COPYRIGHT Lesterbor *****END OF FILE*************/

