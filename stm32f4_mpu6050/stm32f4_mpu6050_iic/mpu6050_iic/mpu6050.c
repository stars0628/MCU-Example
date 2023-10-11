//mpu6050.c
#include "mpu6050.h"
#include <math.h>

mpu6050 Mpu6050_Dat = {0};


/* дһ���ֽ� */
uint8_t MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
    if(HAL_I2C_Mem_Write(&MPU6050_I2C, MPU6050_SLAVE_ADDRESS, reg_add, I2C_MEMADD_SIZE_8BIT, &reg_dat, 1, 0xff) == HAL_OK)
    {
        HAL_Delay(1);
        return SUCCESS;
    }
    return ERROR;
}

/* ��ȡһ���ֽ� */
uint8_t MPU6050_ReadData(uint8_t reg_add,unsigned char* Read,uint8_t num)
{
    if(HAL_I2C_Mem_Read(&MPU6050_I2C, MPU6050_SLAVE_ADDRESS | 0x01, reg_add, I2C_MEMADD_SIZE_8BIT, Read, num, 0xff) == HAL_OK)
    {
        HAL_Delay(1);
        return SUCCESS;
    }
    return ERROR;
}

/* ��ʼ�� */
void MPU6050_Init(void)
{
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
		HAL_Delay(200);//������ߺ��ӳ�>100ms
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x03);			//ѡ��ʱ��
		MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV, 0x00);	     //�����ǲ����ʣ�1KHz
    MPU6050_WriteReg(MPU6050_RA_CONFIG, 0x03);	         //��ͨ�˲��������ã����ٶ�44hz�˲���������42hz�˲�
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG, 0x08);	   //���ü��ٶȴ�����������4Gģʽ�����Լ�
    MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/* ��ȡID 0x68 */
uint8_t MPU6050ReadID(void)
{
    unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);
    return Re;
}

/* ��ȡԭʼ������ٶ� */
uint8_t MPU6050ReadAcc(mpu6050 *data)
{
    uint8_t buf[6];
    if(!MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6))
    {
        data->ax = (short)(buf[0] << 8) | buf[1];
        data->ay = (short)(buf[2] << 8) | buf[3];
        data->az = (short)(buf[4] << 8) | buf[5];
        return SUCCESS;
    }
    return ERROR;
}

/* ��ȡԭʼ������ٶ� */
uint8_t MPU6050ReadGyro(mpu6050 *data)
{
    uint8_t buf[6];
    if(!MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6))
    {
        data->gx = (short)(buf[0] << 8) | buf[1];
        data->gy = (short)(buf[2] << 8) | buf[3];
        data->gz = (short)(buf[4] << 8) | buf[5];
        return SUCCESS;
    }
    return ERROR;
}

/* ��ȡ�¶� */
uint8_t MPU6050_ReturnTemp(mpu6050 *data)
{
    short temp3;
    uint8_t buf[2];
    if(!MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2))
    {
        temp3= (buf[0] << 8) | buf[1];
        data->Temperature =((double) temp3/340.0)+36.53;
        return SUCCESS;
    }
    return ERROR;
}
