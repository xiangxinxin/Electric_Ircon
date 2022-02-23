
#ifndef	__ROTARY_ENCODER_H
#define	__ROTARY_ENCODER_H

#include	"config.h"

//sig:  1:Encoder_PinA引脚变化 2：Encoder_PinB引脚变化
//return: 0:无变化  1：正转1步  2：反转1步
extern uint8_t encoder_loop(void);

#endif
