
#ifndef __CONFIG_H
#define __CONFIG_H

/*********************************************************/
// #define MAIN_Fosc 5529600L  //������ʱ��
// #define MAIN_Fosc 6000000L  //������ʱ��
// #define MAIN_Fosc 11059200L //������ʱ��
// #define MAIN_Fosc 18432000L //������ʱ��
// #define MAIN_Fosc 20000000L //������ʱ��
// #define MAIN_Fosc 22118400L //������ʱ��
#define MAIN_Fosc 24000000L //������ʱ��
// #define MAIN_Fosc 27000000L //������ʱ��
// #define MAIN_Fosc 30000000L //������ʱ��
// #define MAIN_Fosc 33177600L //������ʱ��
// #define MAIN_Fosc 35000000L //������ʱ��
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
extern volatile uint32_t time_cnt_inc; //�Զ���һ��ϵͳ�ܹ��õı���

extern uint8_t IRCBAND_temp;
extern uint8_t LIRTRIM_temp;
extern uint8_t IRTRIM_temp;
extern uint8_t CLKDIV_temp;

#endif
