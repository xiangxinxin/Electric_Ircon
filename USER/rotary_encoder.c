

#include "rotary_encoder.h"

typedef enum
{
	ENCODER_STEP1 = 0,
	ENCODER_STEP2,
	ENCODER_STEP3,
	ENCODER_STEP4
} _ENCODER_LOOP;

_ENCODER_LOOP encoder_loop_step = ENCODER_STEP1;

//sig:  1:Encoder_PinA引脚变化 2：Encoder_PinB引脚变化
//return: 0:无变化  1：正转1步  2：反转1步
uint8_t encoder_loop(void)
{
	static uint8_t step_cnt = 10;
	switch (encoder_loop_step)
	{
	case ENCODER_STEP1:
	{
		if (sigA.event.sig_falling) //下降沿
		{
			encoder_loop_step = ENCODER_STEP2;
			step_cnt++;
		}
		else if (sigB.event.sig_falling) //下降沿
		{
			encoder_loop_step = ENCODER_STEP4;
			step_cnt--;
		}
	}
	break;
	case ENCODER_STEP2:
	{
		if (sigA.event.sig_rising) //上升沿
		{
			encoder_loop_step = ENCODER_STEP1;
			step_cnt--;
		}
		else if (sigB.event.sig_falling) //下降沿
		{
			encoder_loop_step = ENCODER_STEP3;
			step_cnt++;
		}
	}
	break;
	case ENCODER_STEP3:
	{
		if (sigA.event.sig_rising) //上升沿
		{
			encoder_loop_step = ENCODER_STEP4;
			step_cnt++;
		}
		else if (sigB.event.sig_rising) //上升沿
		{
			encoder_loop_step = ENCODER_STEP2;
			step_cnt--;
		}
	}
	break;
	case ENCODER_STEP4:
	{
		if (sigA.event.sig_falling) //下降沿
		{
			encoder_loop_step = ENCODER_STEP3;
			step_cnt--;
		}
		else if (sigB.event.sig_rising) //上升沿
		{
			encoder_loop_step = ENCODER_STEP1;
			step_cnt++;
		}
	}
	break;
	default:
		break;
	}
	
	sigA.event.sig_rising = 0;
	sigA.event.sig_falling = 0;
	sigB.event.sig_rising = 0;
	sigB.event.sig_falling = 0;
	if (step_cnt >= 10 + 4)
	{
		step_cnt = 10;
		return 1;
	}
	else if (step_cnt <= 10 - 4)
	{
		step_cnt = 10;
		return 2;
	}

	return 0;
}
