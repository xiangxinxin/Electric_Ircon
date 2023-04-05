
#ifndef __PID_H
#define __PID_H

#include "config.h"

struct _pid
{
    float SetValue;    //定义设定值
    float ActualValue; //定义实际值
    float err;         //定义偏差值
    float err_last;    //定义上一个偏差值
    float Kp, Ki, Kd;  //定义比例、积分、微分系数
    float voltage;     //定义电压值（控制执行器的变量）
    float integral;    //定义积分值
};

extern struct _pid pid;

float PID_Operation(float SetValue, float ActualValue);
void PID_init(void); // PID参数初始化

#endif
