/*
 * @Description: 
 * @Author: 
 * @Change Logs: 
 * @Date            Author        Notes: 
 * 2018-3-27      the first version     
 */

#ifndef __MULTI_KEY_H
#define __MULTI_KEY_H

#include "Config.h"
#include "macro.h"


#define KEYNUM 2			  //按键数量
#define KEY_FILTER_TIME 1	  // (max:8)
#define KEY_LONG_TIME 50	  // 单位10ms， 持续1秒，认为长按事件
#define KEY_REPEAT_TIME 25	  //
#define KEY_MODE 0			  //0:多按键同时触发 1：单独按键触发，按键按下不释放，其他按键不释放，长按除外
#define SET_LONG_PRESS_TIME 0 //设置单个按键长按时间

typedef enum
{
	DEBOUNCE = 0,	 // 等待
	PressStatus,	 // 有按键按下
	LongPressStatus, //长按状态
	RepeatStatus,	 //重复状态
} Key_status_enum;	 //定义了按键状态 用于按键扫描标记扫描步骤

typedef enum
{
	KEY_DOWN = 0,
	KEY_UP,
	KEY_PRESSED,
	KEY_LONG_PRESSED,
	KEY_REPEAT,
} key_status_t;

typedef struct KEY_EVENT
{
	uint8_t key_down : 1;		   //按键按下
	uint8_t key_up : 1;			   //按键松开
	uint8_t click_up : 1;		   //点击 松手有效
	uint8_t click_down : 1;		   //点击 按下有效
	uint8_t key_long_pressed : 1;  //按键长按
	uint8_t key_repeat : 1;		   //按键重复
	uint8_t valid : 1;			   //有效
} KEY_EVENT_T;

//定义一个函数指针 返回uint8_t 无参数输入
//原型：uint8_t(*)(void)
//别名：is_key_down_cb
// typedef uint8_t		(code *is_key_down_cb)(void);//是否有按键按下
typedef struct MULTI_KEY
{
	uint8_t filter_conut : 4; //消抖计数
	uint8_t scan_status : 3;  //扫描状态
	uint8_t is_key_down : 1;  //判断是否按下 在外部由用户处理
	uint8_t repeat_speed : 4; //重复触发速度
	uint8_t repeat_count : 4; //重复触发计数
	KEY_EVENT_T event;		  //按键事件
	uint8_t long_count;		  //长按计数
	uint8_t long_time;		  //长按计时
} MULTI_KEY_T;

extern   MULTI_KEY_T key_head_handle[];
extern void multi_key_init(void);
extern void key_stop(MULTI_KEY_T *handle);
extern void key_scan(void);
extern void set_long_press_time(MULTI_KEY_T *handle, uint16_t long_time);

#endif
