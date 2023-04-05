#include "pid.h"

struct _pid pid;

// PID参数初始化
void PID_init(void)
{
    pid.SetValue = 0.0;    //设定值
    pid.ActualValue = 0.0; //实际值
    pid.err = 0.0;         //偏差
    pid.err_last = 0.0;    //上一个偏差值
    pid.voltage = 0.0;     //电压值（控制执行器的变量）
    pid.integral = 0.0;    //积分值

    pid.Kp = 0.07;  //比例系数
    pid.Ki = 0.003; //积分系数
    pid.Kd = 0;     //微分系数
}

// PID运算  位置式
float PID_Operation(float SetValue, float ActualValue)
{
    pid.SetValue = SetValue;                  //设定值
    pid.ActualValue = ActualValue;            //实际值
    pid.err = pid.SetValue - pid.ActualValue; //计算这一次的误差
    pid.integral += pid.err;                  //误差积分(积分部分使用)
    if (pid.integral >= 10000)
        pid.integral = 10000;
    else if (pid.integral < -100)
        pid.integral = -100;
    pid.voltage = pid.Kp * pid.err + pid.Ki * pid.integral; //核心PI运算

    return pid.voltage;
}

// // PID运算  位置式
// float PID_Operation(float SetValue, float ActualValue)
// {
//     pid.SetValue = SetValue;                  //设定值
//     pid.ActualValue = ActualValue;            //实际值
//     pid.err = pid.SetValue - pid.ActualValue; //计算这一次的误差
//     pid.integral += pid.err;                  //误差积分(积分部分使用)
//     if (pid.integral >= 10000)
//         pid.integral = 10000;
//     else if (pid.integral <= -10000)
//         pid.integral = -10000;
//     pid.voltage = pid.Kp * pid.err + pid.Ki * pid.integral; //核心PI运算

//     return pid.voltage;
// }

////PID运算  增量式
// float PID_Operation(float SetValue,float ActualValue){
//     pid.SetValue=SetValue;             //设定值
//	  pid.ActualValue=ActualValue;    //实际值
//     pid.err=pid.SetValue-pid.ActualValue;      //计算这一次的误差
//
//     pid.integral+=pid.err;             //误差积分(积分部分使用)
//
//     pid.voltage = pid.Kp*(pid.err-pid.err_last)+pid.Ki*pid.err;//核心PI运算
//
//	  pid.err_last=pid.err;            //更新上一次误差(微分部分使用)
//     return pid.voltage;
// }

// PID运算
/*
float PID_realize(float SetValue){
    pid.SetValue=SetValue;             //设定值
    pid.err=pid.SetValue-pid.ActualValue;      //计算这一次的误差
    pid.integral+=pid.err;             //误差积分(积分部分使用)
    pid.voltage=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);//核心PID运算
    pid.err_last=pid.err;            //更新上一次误差(微分部分使用)

//	  pid.ActualValue=pid.voltage*1.0;   //更新实际值
    return pid.voltage;
}
*/
