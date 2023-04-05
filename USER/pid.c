#include "pid.h"

struct _pid pid;

// PID������ʼ��
void PID_init(void)
{
    pid.SetValue = 0.0;    //�趨ֵ
    pid.ActualValue = 0.0; //ʵ��ֵ
    pid.err = 0.0;         //ƫ��
    pid.err_last = 0.0;    //��һ��ƫ��ֵ
    pid.voltage = 0.0;     //��ѹֵ������ִ�����ı�����
    pid.integral = 0.0;    //����ֵ

    pid.Kp = 0.07;  //����ϵ��
    pid.Ki = 0.003; //����ϵ��
    pid.Kd = 0;     //΢��ϵ��
}

// PID����  λ��ʽ
float PID_Operation(float SetValue, float ActualValue)
{
    pid.SetValue = SetValue;                  //�趨ֵ
    pid.ActualValue = ActualValue;            //ʵ��ֵ
    pid.err = pid.SetValue - pid.ActualValue; //������һ�ε����
    pid.integral += pid.err;                  //������(���ֲ���ʹ��)
    if (pid.integral >= 10000)
        pid.integral = 10000;
    else if (pid.integral < -100)
        pid.integral = -100;
    pid.voltage = pid.Kp * pid.err + pid.Ki * pid.integral; //����PI����

    return pid.voltage;
}

// // PID����  λ��ʽ
// float PID_Operation(float SetValue, float ActualValue)
// {
//     pid.SetValue = SetValue;                  //�趨ֵ
//     pid.ActualValue = ActualValue;            //ʵ��ֵ
//     pid.err = pid.SetValue - pid.ActualValue; //������һ�ε����
//     pid.integral += pid.err;                  //������(���ֲ���ʹ��)
//     if (pid.integral >= 10000)
//         pid.integral = 10000;
//     else if (pid.integral <= -10000)
//         pid.integral = -10000;
//     pid.voltage = pid.Kp * pid.err + pid.Ki * pid.integral; //����PI����

//     return pid.voltage;
// }

////PID����  ����ʽ
// float PID_Operation(float SetValue,float ActualValue){
//     pid.SetValue=SetValue;             //�趨ֵ
//	  pid.ActualValue=ActualValue;    //ʵ��ֵ
//     pid.err=pid.SetValue-pid.ActualValue;      //������һ�ε����
//
//     pid.integral+=pid.err;             //������(���ֲ���ʹ��)
//
//     pid.voltage = pid.Kp*(pid.err-pid.err_last)+pid.Ki*pid.err;//����PI����
//
//	  pid.err_last=pid.err;            //������һ�����(΢�ֲ���ʹ��)
//     return pid.voltage;
// }

// PID����
/*
float PID_realize(float SetValue){
    pid.SetValue=SetValue;             //�趨ֵ
    pid.err=pid.SetValue-pid.ActualValue;      //������һ�ε����
    pid.integral+=pid.err;             //������(���ֲ���ʹ��)
    pid.voltage=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);//����PID����
    pid.err_last=pid.err;            //������һ�����(΢�ֲ���ʹ��)

//	  pid.ActualValue=pid.voltage*1.0;   //����ʵ��ֵ
    return pid.voltage;
}
*/
