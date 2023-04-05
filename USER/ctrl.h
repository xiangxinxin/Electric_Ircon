

#ifndef __CTRL_H
#define __CTRL_H

#include "config.h"

#define RTT_DEBUG 0

//__VA_ARGS__  ：可变参数
#define RTT_LOG_DEBUG(...)                 \
    if (RTT_DEBUG)                         \
    {                                      \
        SEGGER_RTT_printf(0, __VA_ARGS__); \
    }

typedef struct
{
    uint8_t enable;     //使能
    uint8_t period;     //周期
    uint8_t period_cnt; //周期计数
    uint8_t duty_cycle; //占空比
} _PWM_T;

typedef struct
{
    uint8_t heat_onoff : 1;       //加热开关
    int16_t encoder_cnt;          //编码器计数
    uint16_t inside_val;          //内部电压
    uint16_t vcc_voltage;         // VCC电压
    uint16_t vbus_voltage;        // VBUS电压
    uint16_t t12_voltage;         // T12电烙铁电压
    uint16_t t12_temperature;     // T12电烙铁温度
    uint16_t t12_set_temperature; // T12电烙铁设定温度
    uint16_t BGA_val;             // MCU内部电压测量值
    uint16_t pwm_val;             // PWM值
    uint16_t encoder_A_rising;    //上升沿
    uint16_t encoder_A_falling;   //下降沿
    uint16_t encoder_B_rising;    //上升沿
    uint16_t encoder_B_falling;   //下降沿
    uint16_t encoder_setp_cnt;    //
    uint16_t encoder_loop;        //
} _CTRL_T;

extern _PWM_T pwm1;
extern uint16_t temperature_list[]; // t12 热电偶分度表
extern _CTRL_T ctrl;

void ctrl_main(void);
extern int ADCRead(uint8_t channel);
extern void PWM_irq(void); //中断中处理PWM
extern void ADC_Process(void);
extern void heat_Process(void); //加热进程
extern void display_Process(void);
extern void SetPWM(uint16_t pwm_val);

#endif
