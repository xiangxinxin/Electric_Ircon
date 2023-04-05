
#ifndef __PID_H
#define __PID_H

#include "config.h"

struct _pid
{
    float SetValue;    //�����趨ֵ
    float ActualValue; //����ʵ��ֵ
    float err;         //����ƫ��ֵ
    float err_last;    //������һ��ƫ��ֵ
    float Kp, Ki, Kd;  //������������֡�΢��ϵ��
    float voltage;     //�����ѹֵ������ִ�����ı�����
    float integral;    //�������ֵ
};

extern struct _pid pid;

float PID_Operation(float SetValue, float ActualValue);
void PID_init(void); // PID������ʼ��

#endif
