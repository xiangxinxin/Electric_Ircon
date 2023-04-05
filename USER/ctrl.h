

#ifndef __CTRL_H
#define __CTRL_H

#include "config.h"

#define RTT_DEBUG 0

//__VA_ARGS__  ���ɱ����
#define RTT_LOG_DEBUG(...)                 \
    if (RTT_DEBUG)                         \
    {                                      \
        SEGGER_RTT_printf(0, __VA_ARGS__); \
    }

typedef struct
{
    uint8_t enable;     //ʹ��
    uint8_t period;     //����
    uint8_t period_cnt; //���ڼ���
    uint8_t duty_cycle; //ռ�ձ�
} _PWM_T;

typedef struct
{
    uint8_t heat_onoff : 1;       //���ȿ���
    int16_t encoder_cnt;          //����������
    uint16_t inside_val;          //�ڲ���ѹ
    uint16_t vcc_voltage;         // VCC��ѹ
    uint16_t vbus_voltage;        // VBUS��ѹ
    uint16_t t12_voltage;         // T12��������ѹ
    uint16_t t12_temperature;     // T12�������¶�
    uint16_t t12_set_temperature; // T12�������趨�¶�
    uint16_t BGA_val;             // MCU�ڲ���ѹ����ֵ
    uint16_t pwm_val;             // PWMֵ
    uint16_t encoder_A_rising;    //������
    uint16_t encoder_A_falling;   //�½���
    uint16_t encoder_B_rising;    //������
    uint16_t encoder_B_falling;   //�½���
    uint16_t encoder_setp_cnt;    //
    uint16_t encoder_loop;        //
} _CTRL_T;

extern _PWM_T pwm1;
extern uint16_t temperature_list[]; // t12 �ȵ�ż�ֶȱ�
extern _CTRL_T ctrl;

void ctrl_main(void);
extern int ADCRead(uint8_t channel);
extern void PWM_irq(void); //�ж��д���PWM
extern void ADC_Process(void);
extern void heat_Process(void); //���Ƚ���
extern void display_Process(void);
extern void SetPWM(uint16_t pwm_val);

#endif
