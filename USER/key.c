#include "key.h"

#define DATA_CLCLE_FLAG 0x80 //����ѭ��
#define DATA_MINUS 0		 //����
#define DATA_PLUS 1			 //��������
/*****************************************************
*_8bit_set_data_para
*�������ܣ��޸�8bit����
*��ڲ�����p_val����Ҫ�޸�ֵ��ָ��  max������
		min:���� status���ӻ��  step���Ӽ�����
*���ڲ�������
*****************************************************/
void _8bit_set_data_para(uint8_t *p_val, uint8_t max, uint8_t min, uint8_t status, uint8_t step)
{
	if (p_val == (void *)0)
		return;

	if (min > max)
		return;

	if (status & DATA_PLUS) //�ж���������
	{
		(*p_val) += step;
		if ((*p_val) > max)
		{
			if (status & DATA_CLCLE_FLAG)
			{
				(*p_val) = min;
			} //�ж��Ƿ�ѭ��
			else
			{
				(*p_val) = max;
			}
		}
	}
	else
	{
		if ((*p_val) > min)
		{
			(*p_val) -= step;
		}
		else if (status & DATA_CLCLE_FLAG)
		{
			(*p_val) = max;
		} //�ж��Ƿ�ѭ��
		else
		{
			(*p_val) = min;
		}
	}

	//������ж���ֵ�Ƿ���������Χ
	if ((*p_val) > max)
	{
		(*p_val) = max;
	}
	else if ((*p_val) < min)
	{
		(*p_val) = min;
	}
}

/************************************************************
** ��������: K E Y _ P R O C E S S
** ��������:
** �������:
** �������:
** �� �� ֵ:
** ��    ��:
** ��    ��:
** ��    ��:
** �޸�����  �汾��   �޸���  �޸�����
**************************************************************/
void key_process(void)
{
	if (key1.event.click_up)
	{
		if (ctrl.heat_onoff)
		{
			ctrl.heat_onoff = 0;
			ctrl.encoder_cnt = 0;
		}
		else
		{
			ctrl.heat_onoff = 1;
		}
	}

	if (key1.event.key_long_pressed)
	{
		key1.event.key_long_pressed = 0;
	}
}

/**************************************************
 *�������ƣ�void  Sys_Scan(void)
 *�������ܣ�ɨ��TK
 *��ڲ�����void
 *���ڲ�����void
 **************************************************/
void Sys_Scan(void)
{
	if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
		key1.is_key_down = 1;
	else
		key1.is_key_down = 0;

	if (HAL_GPIO_ReadPin(T12_KEY_GPIO_Port, T12_KEY_Pin) == GPIO_PIN_RESET)
		key2.is_key_down = 1;
	else
		key2.is_key_down = 0;
}
