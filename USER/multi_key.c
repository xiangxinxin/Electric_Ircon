/*
 * @Description: 
 * @Author: 
 * @Change Logs: 
 * @Date            Author        Notes: 
 * 2010-10-12      
 */

#include "multi_key.h"

// MULTI_KEY_T *key_head_handle = NULL;
  MULTI_KEY_T key_head_handle[KEYNUM] = {0};

/************************************************************
** ��������: M U L T I _ K E Y _ I N I T
** ��������: 
** �������:
** �������:
** �� �� ֵ:
** ��    ��:  
** ��    ��:  
** ��    ��: 
** �޸�����  �汾��   �޸���  �޸�����
**************************************************************/
void multi_key_init(void)
{
      uint8_t i;
    for (i = 0; i < KEYNUM; i++) //ɨ����ÿ������
    {
        key_head_handle[i].is_key_down = 0;
        key_head_handle[i].filter_conut = 0;
        key_head_handle[i].scan_status = DEBOUNCE;         //�ȴ�״̬
        key_head_handle[i].repeat_speed = KEY_REPEAT_TIME; //�ظ�ʱ��
        key_head_handle[i].repeat_count = 0;
        key_head_handle[i].long_count = 0;
        key_head_handle[i].long_time = KEY_LONG_TIME; //����ʱ��
        key_head_handle[i].event.key_down = 0;
        key_head_handle[i].event.key_up = 1;
        key_head_handle[i].event.click_down = 0; //��� ������Ч
        key_head_handle[i].event.click_up = 0;   //��� ������Ч
        key_head_handle[i].event.key_long_pressed = 0;
        key_head_handle[i].event.key_repeat = 0;
        key_head_handle[i].event.valid = 0; //��Ч
    }
}

#if KEY_MODE
/**
 * @description: ������а������Ƿ�ȫ���ɿ�
 * @param {type} 
 * @return: 
 */
static uint8_t check_has_key_press(void)
{

      uint8_t i;
    bit is_key_up_flag = FALSE;
    for (i = 0; i < KEYNUM; i++) //ɨ����ÿ������
    {
        if (key_head_handle[i].event.key_up)
        {
            is_key_up_flag = TRUE;
        }
        else
        {
            is_key_up_flag = FALSE;
            break;
        }
    }
    return is_key_up_flag;
}
#endif
/**
 * @description: ɨ�谴��
 * @param {type} 
 * @return: 
 */
void key_scan(void)
{
    // MULTI_KEY_T *handle;
      uint8_t i;
#if KEY_MODE
    static bit none_key_pressed_flag = FALSE;
    none_key_pressed_flag = check_has_key_press(); //�ж��Ƿ��а���up
#endif
    // for (handle = key_head_handle; handle != NULL; handle = key_head_handle[i].next)//ɨ��������ÿ������
    for (i = 0; i < KEYNUM; i++) //ɨ����ÿ������
    {
        switch (key_head_handle[i].scan_status)
        {
        case DEBOUNCE: // �ȴ�
            //key_head_handle[i].is_key_down()����ͨ������ָ����á�static is_reset_wash_key_press��void��������
            if (key_head_handle[i].is_key_down) //�жϵײ�������Ƿ��а������� �жϵ���key_value�е�����
            {
                key_head_handle[i].filter_conut++;
                if (key_head_handle[i].filter_conut >= KEY_FILTER_TIME) //�������¼���>=�����˲������������˲���
                {
                    key_head_handle[i].scan_status = PressStatus;      //��ǽ��밴��״̬
                    key_head_handle[i].filter_conut = KEY_FILTER_TIME; //�������¼���=�趨���˲�����
#if KEY_MODE                                                           //KEY_MODE=1������������ģʽ
                    if (none_key_pressed_flag)                         //�ж����а������ǲ������а������ɿ���
                    {
                        key_head_handle[i].event.valid = 1; //û�а������� ������߼��ж���Ч
                    }
                    else
                    {
                        key_head_handle[i].event.valid = 0; //�а������� ������߼��ж���Ч
                    }
#endif
                }
            }
            else
            {
                key_head_handle[i].filter_conut = 0; //���㰴������ɨ���������
                // if (key_head_handle[i].event.key_pressed) //����ж�ѹ�¼�
                {
                    // key_head_handle[i].event.key_pressed = 0; //�����ѹ�¼����
                    key_head_handle[i].event.key_up = 1; //��ǰ����ɿ����
                }
                key_head_handle[i].event.key_down = 0;   //����������±��
                key_head_handle[i].event.click_down = 0; //��� ������Ч
                key_head_handle[i].event.click_up = 0;   //��� ������Ч
                key_head_handle[i].event.key_long_pressed = 0;      //�����ѹ���
                key_head_handle[i].event.key_repeat = 0;            //����ظ����
            }
            key_head_handle[i].long_count = 0;   //���㳤������
            key_head_handle[i].repeat_count = 0; //�����ظ�����
            break;

        case PressStatus:                       //����ѹ��״̬
            if (key_head_handle[i].is_key_down) //�жϵײ�������Ƿ��а�������
            {
                key_head_handle[i].event.key_up = 0; //������ֱ�־
                                                     // key_head_handle[i].event.key_pressed = 1; //��λ��ѹ��־
#if KEY_MODE
                if (key_head_handle[i].event.valid) //�¼��Ƿ���Ч
                {
                    key_head_handle[i].event.key_down = 1;   //��ǰ��������¼���������
                    key_head_handle[i].event.click_down = 1; //��� ������Ч
                }
#else
                key_head_handle[i].event.key_down = 1;           //��ǰ��������¼���������
                key_head_handle[i].event.click_down = 1;         //��� ������Ч
#endif
                key_head_handle[i].scan_status = LongPressStatus; //����ɨ��״̬Ϊ����״̬
            }
            else
            {
                key_head_handle[i].scan_status = DEBOUNCE; //��Ϊ�ȴ�״̬
            }
            break;

        case LongPressStatus:                        //����״̬
            key_head_handle[i].event.click_down = 0; //��� ������Ч
            if (key_head_handle[i].is_key_down)      //�жϵײ�������Ƿ��а�������
            {
                if (key_head_handle[i].long_time > 0) //������ʱ>0
                {
                    if (++key_head_handle[i].long_count >= key_head_handle[i].long_time) //������ʱ�Լ�ͬʱ�ж����趨ֵ�Ĵ�С
                    {
                        key_head_handle[i].event.key_long_pressed = 1; //��ǳ����¼�
                        key_head_handle[i].scan_status = RepeatStatus; //�����ظ�״̬
                    }
                }
            }
            else
            {
                key_head_handle[i].event.click_up = 1;     //��� ������Ч
                key_head_handle[i].scan_status = DEBOUNCE; //��ǽ���ȴ�״̬
            }
            break;

        case RepeatStatus:                      //�ظ�״̬
            if (key_head_handle[i].is_key_down) //�ж��Ƿ���
            {
                if (key_head_handle[i].repeat_speed > 0) //�ظ���ʱ>0
                {
                    if (++key_head_handle[i].repeat_count >= key_head_handle[i].repeat_speed) //�ظ�������ʱ�����趨ֵ
                    {
                        key_head_handle[i].repeat_count = 0; //�ظ�������ʱ����
#if KEY_MODE
                        if (key_head_handle[i].event.valid) //�¼��Ƿ���Ч
                        {
                            key_head_handle[i].event.key_repeat = 1; //����ظ��¼�
                        }
#else
                        key_head_handle[i].event.key_repeat = 1; //����ظ��¼�
#endif
                    }
                }
            }
            else
            {
                key_head_handle[i].scan_status = DEBOUNCE; //��Ϊ�ȴ�״̬
            }
            break;

        default:
            key_head_handle[i].scan_status = DEBOUNCE; //��Ϊ�ȴ�״̬
            break;
        }
    }
}
