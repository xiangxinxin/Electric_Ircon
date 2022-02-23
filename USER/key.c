#include "key.h"

#define DATA_CLCLE_FLAG 0x80 //数据循环
#define DATA_MINUS 0		 //数据
#define DATA_PLUS 1			 //数据增减
/*****************************************************
*_8bit_set_data_para
*函数功能：修改8bit参数
*入口参数：p_val：需要修改值的指针  max：上限
		min:下限 status：加或减  step：加减步进
*出口参数：无
*****************************************************/
void _8bit_set_data_para(uint8_t *p_val, uint8_t max, uint8_t min, uint8_t status, uint8_t step)
{
	if (p_val == (void *)0)
		return;

	if (min > max)
		return;

	if (status & DATA_PLUS) //判断数据增减
	{
		(*p_val) += step;
		if ((*p_val) > max)
		{
			if (status & DATA_CLCLE_FLAG)
			{
				(*p_val) = min;
			} //判断是否循环
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
		} //判断是否循环
		else
		{
			(*p_val) = min;
		}
	}

	//最后再判断数值是否在正常范围
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
** 函数名称: K E Y _ P R O C E S S
** 功能描述:
** 输入参数:
** 输出参数:
** 返 回 值:
** 作    者:
** 日    期:
** 版    本:
** 修改日期  版本号   修改人  修改内容
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
 *函数名称：void  Sys_Scan(void)
 *函数功能：扫描TK
 *入口参数：void
 *出口参数：void
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
