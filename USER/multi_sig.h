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

#define SIG_NUM 2         //按键数量
#define SIG_FILTER_TIME 2 // (max:8)

typedef enum
{
    PRESS_UP = 0,  //
    PRESS_DOWN,    //
} Sig_status_enum; //定义了按键状态 用于按键扫描标记扫描步骤

typedef enum
{
    SIG_DOWN = 0,
    SIG_UP,
} sig_status_t;

typedef struct SIG_EVENT
{
    uint8_t sig_down;    //按键按下
    uint8_t sig_up;      //按键松开
    uint8_t sig_rising;  //上升沿
    uint8_t sig_falling; //下降沿
} SIG_EVENT_T;

typedef struct MULTI_SIG
{
    uint8_t is_sig_down;         //判断是否按下 在外部由用户处理
    Sig_status_enum scan_status; //扫描状态
    uint8_t filter_conut;        //消抖计数
    SIG_EVENT_T event;           //按键事件
} MULTI_SIG_T;

extern   MULTI_SIG_T sig_array[];
extern void multi_sig_init(void);
extern void sig_scan(void);

#define sigA sig_array[0]
#define sigB sig_array[1]

#endif
