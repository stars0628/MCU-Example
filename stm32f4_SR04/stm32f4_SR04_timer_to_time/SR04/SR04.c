//SR04.c
#include "SR04.h"

float distant;      //��������
uint32_t high_time;   //������ģ�鷵�صĸߵ�ƽʱ��


void for_delay_us(uint16_t us)//΢�뼶��ʱ��ѭ����
{
		uint8_t system_clock = 168;//��Ƶ
		uint16_t nus = us * system_clock;
		do
		{
			__NOP();
		}
		while(nus--);
}


void  Start(void) //����������ģ��
{
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_SET); //����
    for_delay_us(15);          //15us��ʱ
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_RESET);  //����
}


int Get(void)
{
    return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0); //���Echo�ĵ�ƽ�ߵ�

}

void start(void)//��ʼ��ʱ
{
    __HAL_TIM_SetCounter(&htim_sr04,0);   //��0��ʼ����
    HAL_TIM_Base_Start_IT(&htim_sr04);   // ������ʱ�������ж�
}

unsigned int get_stop(void)//��ȡֹ֮ͣ�������
{
    unsigned int  dta;
    HAL_TIM_Base_Stop_IT (&htim_sr04);       //�رն�ʱ�������ж�
    high_time = __HAL_TIM_GetCounter(&htim_sr04);  //��ü���ֵ
    dta = high_time*340;                     //high_timeΪʱ�䣬��λus  ���赱ǰ�����ٶ�Ϊ340m/s
    return dta;
}

void SR04_GetData(void)
{
		Start();               //����������ģ��
		while(!Get());        //��Echo����0���ȴ�,��Ϊ1����
		start();//��ʼ��ʱ
		while(Get());		  //��EchoΪ1�������ȴ�  ,��Ϊ0����
		distant = (float)get_stop()/(float)20000.0;//��λΪcm
}
