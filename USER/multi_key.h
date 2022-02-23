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


#define KEYNUM 2			  //��������
#define KEY_FILTER_TIME 1	  // (max:8)
#define KEY_LONG_TIME 50	  // ��λ10ms�� ����1�룬��Ϊ�����¼�
#define KEY_REPEAT_TIME 25	  //
#define KEY_MODE 0			  //0:�ఴ��ͬʱ���� 1�����������������������²��ͷţ������������ͷţ���������
#define SET_LONG_PRESS_TIME 0 //���õ�����������ʱ��

typedef enum
{
	DEBOUNCE = 0,	 // �ȴ�
	PressStatus,	 // �а�������
	LongPressStatus, //����״̬
	RepeatStatus,	 //�ظ�״̬
} Key_status_enum;	 //�����˰���״̬ ���ڰ���ɨ����ɨ�貽��

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
	uint8_t key_down : 1;		   //��������
	uint8_t key_up : 1;			   //�����ɿ�
	uint8_t click_up : 1;		   //��� ������Ч
	uint8_t click_down : 1;		   //��� ������Ч
	uint8_t key_long_pressed : 1;  //��������
	uint8_t key_repeat : 1;		   //�����ظ�
	uint8_t valid : 1;			   //��Ч
} KEY_EVENT_T;

//����һ������ָ�� ����uint8_t �޲�������
//ԭ�ͣ�uint8_t(*)(void)
//������is_key_down_cb
// typedef uint8_t		(code *is_key_down_cb)(void);//�Ƿ��а�������
typedef struct MULTI_KEY
{
	uint8_t filter_conut : 4; //��������
	uint8_t scan_status : 3;  //ɨ��״̬
	uint8_t is_key_down : 1;  //�ж��Ƿ��� ���ⲿ���û�����
	uint8_t repeat_speed : 4; //�ظ������ٶ�
	uint8_t repeat_count : 4; //�ظ���������
	KEY_EVENT_T event;		  //�����¼�
	uint8_t long_count;		  //��������
	uint8_t long_time;		  //������ʱ
} MULTI_KEY_T;

extern   MULTI_KEY_T key_head_handle[];
extern void multi_key_init(void);
extern void key_stop(MULTI_KEY_T *handle);
extern void key_scan(void);
extern void set_long_press_time(MULTI_KEY_T *handle, uint16_t long_time);

#endif
