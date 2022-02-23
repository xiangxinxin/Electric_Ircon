
#ifndef __CONFIG_H
#define __CONFIG_H

/*********************************************************/
// #define MAIN_Fosc 5529600L  //定义主时钟
// #define MAIN_Fosc 6000000L  //定义主时钟
// #define MAIN_Fosc 11059200L //定义主时钟
// #define MAIN_Fosc 18432000L //定义主时钟
// #define MAIN_Fosc 20000000L //定义主时钟
// #define MAIN_Fosc 22118400L //定义主时钟
#define MAIN_Fosc 24000000L //定义主时钟
// #define MAIN_Fosc 27000000L //定义主时钟
// #define MAIN_Fosc 30000000L //定义主时钟
// #define MAIN_Fosc 33177600L //定义主时钟
// #define MAIN_Fosc 35000000L //定义主时钟
/*********************************************************/

#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "stdio.h"
#include "stm32f0xx_hal.h"
#include "SEGGER_RTT.h"
#include "rotary_encoder.h"
#include "multi_key.h"
#include "multi_sig.h"
#include "string.h"
#include "spi.h"
#include "macro.h"
#include "oled.h"
#include "key.h"
#include "main.h"
#include "ctrl.h"

#define LED P53
#define KEY1 P50
#define BUZZ P52
#define PWM_IO P51

#define TEST_IO BUZZ

extern uint8_t uart_temp[];
extern volatile uint32_t time_cnt_inc; //自定义一个系统架构用的变量

extern uint8_t IRCBAND_temp;
extern uint8_t LIRTRIM_temp;
extern uint8_t IRTRIM_temp;
extern uint8_t CLKDIV_temp;

#endif
