/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : DS3231.c
* @brief          : DS3231 program body
* @author         : Lesterbor
*	@time			: 2021-09-26
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/
/* USER CODE END Header */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "DS3231Driver.h"
#include "i2c.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PT */

uint8_t DS3231Buffer[1];
uint8_t DS3231TimeBuffer[8];
int8_t ErrorFlag = 0;
/* USER CODE END PT */

/* Function definition -------------------------------------------------------*/
/* USER CODE BEGIN FD */
/**
* @Function name  BCD_DEC
* @Introduce  	BCD(8421)תDEC
* @Parameter		BCD(8421)ֵ	
* @Return 		DECֵ
*/
uint8_t BCD_DEC(uint8_t BCD_Data){
	uint8_t DEC_Data;

	DEC_Data= BCD_Data&0x0f;
	BCD_Data >>= 4;
	BCD_Data &= 0x0f;
	BCD_Data *= 10;
	DEC_Data += BCD_Data;    
	return DEC_Data;
}

/**
* @Function name  DEC_BCD
* @Introduce  	DECתBCD(8421)
* @Parameter		DECֵ	
* @Return 		BCD(8421)ֵ
*/
uint8_t DEC_BCD(uint8_t DEC_Data){
	uint8_t BCD_DataA,BCD_DataB,BCD_Data;
	
	BCD_DataA=DEC_Data/10;
	BCD_DataB=DEC_Data%10;
	BCD_Data=BCD_DataB+(BCD_DataA<<4);
	return BCD_Data;
}
	
/**
  * @Function name  I2C_DS3231_ReadData
  * @Introduce  		DS3231ͨ��I2C��ȡһ���ֽ�
	* @Parameter			ReadAddr I2C��ȡ��ַ
  * @Return 				ʧ�ܷ��� -1 �ɹ����� 0
  */
uint8_t I2C_DS3231_ReadData(uint8_t ReadAddr){
	uint8_t DS3231_DataAddr[1];
	DS3231_DataAddr[0]= ReadAddr;

	if(HAL_I2C_Master_Transmit(&hi2c3,(DS3231_ADDRESS|I2C_WR),DS3231_DataAddr,sizeof(DS3231_DataAddr), 1000) != HAL_OK){
		ErrorFlag = -1;
		return ErrorFlag;
	}
	if(HAL_I2C_Master_Receive(&hi2c3,(DS3231_ADDRESS|I2C_RD),DS3231Buffer,sizeof(DS3231Buffer), 1000) != HAL_OK){
		ErrorFlag = -1;
		return ErrorFlag;
	}
	return ErrorFlag;	
}

/**
* @Function name  I2C_DS3231_WriteData
* @Introduce  	DS3231ͨ��I2Cдһ���ֽ�
* @Parameter		WriteAddr I2Cд���ַ Data I2Cд������
* @Return 		ʧ�ܷ��� -1 �ɹ����� 0
*/
uint8_t I2C_DS3231_WriteData(uint8_t WriteAddr,uint8_t Data){
	uint8_t DS3231_Data[2];
	DS3231_Data[0]	= WriteAddr;
	DS3231_Data[1]	= Data;
	
	if(HAL_I2C_Master_Transmit(&hi2c3,(DS3231_ADDRESS|I2C_WR),DS3231_Data,sizeof(DS3231_Data), 1000) != HAL_OK){
		ErrorFlag = -1;
		return ErrorFlag;
	}
	return ErrorFlag;
}
	
/**
* @Function name  DS3231_SetTime
* @Introduce  	DS3231ͨ��I2C����ʱ��
* @Parameter		Yea ��ݷ�Χ��0~99
*					Mon	�·ݷ�Χ��0~12
*					Dat	��	��Χ��1~31
*					Wee	��	��Χ��1~7
*					Hou	Сʱ��Χ��0~23
*					Min	���ӷ�Χ��0~59
*					Sec	��	��Χ��0~59
* @Return 		NULL
*/
void DS3231_SetTime(uint8_t Yea,uint8_t Mon,uint8_t Dat,uint8_t Wee,uint8_t Hou,uint8_t Min,uint8_t Sec){
	I2C_DS3231_WriteData(Year_Register		,DEC_BCD(Yea));
	I2C_DS3231_WriteData(Month_Register		,DEC_BCD(Mon));
	I2C_DS3231_WriteData(Date_Register		,DEC_BCD(Dat));
	I2C_DS3231_WriteData(Day_Register			,DEC_BCD(Wee));
	I2C_DS3231_WriteData(Hour_Register		,DEC_BCD(Hou));
	I2C_DS3231_WriteData(Minutes_Register	,DEC_BCD(Min));
	I2C_DS3231_WriteData(Seconds_Register	,DEC_BCD(Sec));
}
	
/**
  * @Function name  DS3231_ReadTime
  * @Introduce  	DS3231ͨ��I2C��ȡʱ��
  * @Return 		NULL
  */
void DS3231_ReadTime(void){
	
	I2C_DS3231_ReadData(Seconds_Register);
	DS3231TimeBuffer[7] = BCD_DEC(DS3231Buffer[0]);
	I2C_DS3231_ReadData(Minutes_Register);
	DS3231TimeBuffer[6] = BCD_DEC(DS3231Buffer[0]);
	I2C_DS3231_ReadData(Hour_Register);
	DS3231TimeBuffer[5] = BCD_DEC(DS3231Buffer[0]);
	I2C_DS3231_ReadData(Day_Register);
	DS3231TimeBuffer[4] = BCD_DEC(DS3231Buffer[0]);
	I2C_DS3231_ReadData(Date_Register);
	DS3231TimeBuffer[3] = BCD_DEC(DS3231Buffer[0]);
	I2C_DS3231_ReadData(Month_Register);
	DS3231TimeBuffer[2] = BCD_DEC(DS3231Buffer[0]);
	I2C_DS3231_ReadData(Year_Register);
	DS3231TimeBuffer[1] = BCD_DEC(DS3231Buffer[0]);
}
/**
  * @Function name  DS3231_Temp
  * @Introduce  	DS3231ͨ��I2C��ȡ�¶�
  * @Return 		NULL
  */
void DS3231_Temp(void)
{
	I2C_DS3231_ReadData(DS3231_TEMPERATUREH);
	DS3231TimeBuffer[0] = BCD_DEC(DS3231Buffer[0]);
}
/**
* @Function name  DS3231_ReadTime_ReturnPoint
* @Introduce  	��������ʱ������ͨ��ָ�뷵�� 
* @attention		DS3231_ReadTime_ReturnPoint()[7] ������
*					DS3231_ReadTime_ReturnPoint()[6] ������
*					DS3231_ReadTime_ReturnPoint()[5] ʱ����
*					DS3231_ReadTime_ReturnPoint()[4] ������
*					DS3231_ReadTime_ReturnPoint()[3] ������
*					DS3231_ReadTime_ReturnPoint()[2] ������
*					DS3231_ReadTime_ReturnPoint()[1] ������
*					DS3231_ReadTime_ReturnPoint()[0] ������
* @Return 		NULL
*/
uint8_t *DS3231_ReadTime_ReturnPoint(void){
	return DS3231TimeBuffer;
}

/* USER CODE END FD */
/************************ (C) COPYRIGHT Lesterbor *****END OF FILE****/

