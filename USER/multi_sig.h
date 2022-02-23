/*
 * @Description: 
 * @Author: 
 * @Change Logs: 
 * @Date            Author        Notes: 
 * 2021-4-9      the first version     
 */

#ifndef __MULTI_SIG_H
#define __MULTI_SIG_H

#include "Config.h"
#include "macro.h"

#define SIG_NUM 2         //��������
#define SIG_FILTER_TIME 2 // (max:8)

typedef enum
{
    PRESS_UP = 0,  //
    PRESS_DOWN,    //
} Sig_status_enum; //�����˰���״̬ ���ڰ���ɨ����ɨ�貽��

typedef enum
{
    SIG_DOWN = 0,
    SIG_UP,
} sig_status_t;

typedef struct SIG_EVENT
{
    uint8_t sig_down;    //��������
    uint8_t sig_up;      //�����ɿ�
    uint8_t sig_rising;  //������
    uint8_t sig_falling; //�½���
} SIG_EVENT_T;

typedef struct MULTI_SIG
{
    uint8_t is_sig_down;         //�ж��Ƿ��� ���ⲿ���û�����
    Sig_status_enum scan_status; //ɨ��״̬
    uint8_t filter_conut;        //��������
    SIG_EVENT_T event;           //�����¼�
} MULTI_SIG_T;

extern   MULTI_SIG_T sig_array[];
extern void multi_sig_init(void);
extern void sig_scan(void);

#define sigA sig_array[0]
#define sigB sig_array[1]

#endif
