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
** 函数名称: M U L T I _ K E Y _ I N I T
** 功能描述: 
** 输入参数:
** 输出参数:
** 返 回 值:
** 作    者:  
** 日    期:  
** 版    本: 
** 修改日期  版本号   修改人  修改内容
**************************************************************/
void multi_key_init(void)
{
      uint8_t i;
    for (i = 0; i < KEYNUM; i++) //扫描中每个按键
    {
        key_head_handle[i].is_key_down = 0;
        key_head_handle[i].filter_conut = 0;
        key_head_handle[i].scan_status = DEBOUNCE;         //等待状态
        key_head_handle[i].repeat_speed = KEY_REPEAT_TIME; //重复时间
        key_head_handle[i].repeat_count = 0;
        key_head_handle[i].long_count = 0;
        key_head_handle[i].long_time = KEY_LONG_TIME; //长按时间
        key_head_handle[i].event.key_down = 0;
        key_head_handle[i].event.key_up = 1;
        key_head_handle[i].event.click_down = 0; //点击 按下有效
        key_head_handle[i].event.click_up = 0;   //点击 按下有效
        key_head_handle[i].event.key_long_pressed = 0;
        key_head_handle[i].event.key_repeat = 0;
        key_head_handle[i].event.valid = 0; //无效
    }
}

#if KEY_MODE
/**
 * @description: 检查所有按键中是否全部松开
 * @param {type} 
 * @return: 
 */
static uint8_t check_has_key_press(void)
{

      uint8_t i;
    bit is_key_up_flag = FALSE;
    for (i = 0; i < KEYNUM; i++) //扫描中每个按键
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
 * @description: 扫描按键
 * @param {type} 
 * @return: 
 */
void key_scan(void)
{
    // MULTI_KEY_T *handle;
      uint8_t i;
#if KEY_MODE
    static bit none_key_pressed_flag = FALSE;
    none_key_pressed_flag = check_has_key_press(); //判断是否有按键up
#endif
    // for (handle = key_head_handle; handle != NULL; handle = key_head_handle[i].next)//扫描链表中每个按键
    for (i = 0; i < KEYNUM; i++) //扫描中每个按键
    {
        switch (key_head_handle[i].scan_status)
        {
        case DEBOUNCE: // 等待
            //key_head_handle[i].is_key_down()是在通过函数指针调用“static is_reset_wash_key_press（void）”函数
            if (key_head_handle[i].is_key_down) //判断底层物理层是否有按键按下 判断的是key_value中的数据
            {
                key_head_handle[i].filter_conut++;
                if (key_head_handle[i].filter_conut >= KEY_FILTER_TIME) //按键按下计数>=按键滤波变量（用于滤波）
                {
                    key_head_handle[i].scan_status = PressStatus;      //标记进入按下状态
                    key_head_handle[i].filter_conut = KEY_FILTER_TIME; //按键按下计数=设定的滤波变量
#if KEY_MODE                                                           //KEY_MODE=1：单按键触发模式
                    if (none_key_pressed_flag)                         //判断所有按键中是不是所有按键都松开了
                    {
                        key_head_handle[i].event.valid = 1; //没有按键按下 后面的逻辑判断有效
                    }
                    else
                    {
                        key_head_handle[i].event.valid = 0; //有按键按下 后面的逻辑判断无效
                    }
#endif
                }
            }
            else
            {
                key_head_handle[i].filter_conut = 0; //清零按键按下扫描次数计数
                // if (key_head_handle[i].event.key_pressed) //如果有短压事件
                {
                    // key_head_handle[i].event.key_pressed = 0; //清除短压事件标记
                    key_head_handle[i].event.key_up = 1; //标记按键松开标记
                }
                key_head_handle[i].event.key_down = 0;   //清除按键按下标记
                key_head_handle[i].event.click_down = 0; //点击 按下有效
                key_head_handle[i].event.click_up = 0;   //点击 松手有效
                key_head_handle[i].event.key_long_pressed = 0;      //清除长压标记
                key_head_handle[i].event.key_repeat = 0;            //清除重复标记
            }
            key_head_handle[i].long_count = 0;   //清零长按计数
            key_head_handle[i].repeat_count = 0; //清零重复计数
            break;

        case PressStatus:                       //按键压下状态
            if (key_head_handle[i].is_key_down) //判断底层物理层是否有按键按下
            {
                key_head_handle[i].event.key_up = 0; //清除松手标志
                                                     // key_head_handle[i].event.key_pressed = 1; //置位短压标志
#if KEY_MODE
                if (key_head_handle[i].event.valid) //事件是否有效
                {
                    key_head_handle[i].event.key_down = 1;   //标记按键按下事件（触发）
                    key_head_handle[i].event.click_down = 1; //点击 按下有效
                }
#else
                key_head_handle[i].event.key_down = 1;           //标记按键按下事件（触发）
                key_head_handle[i].event.click_down = 1;         //点击 按下有效
#endif
                key_head_handle[i].scan_status = LongPressStatus; //设置扫描状态为长按状态
            }
            else
            {
                key_head_handle[i].scan_status = DEBOUNCE; //设为等待状态
            }
            break;

        case LongPressStatus:                        //长按状态
            key_head_handle[i].event.click_down = 0; //点击 按下有效
            if (key_head_handle[i].is_key_down)      //判断底层物理层是否有按键按下
            {
                if (key_head_handle[i].long_time > 0) //长按计时>0
                {
                    if (++key_head_handle[i].long_count >= key_head_handle[i].long_time) //长按计时自加同时判断与设定值的大小
                    {
                        key_head_handle[i].event.key_long_pressed = 1; //标记长按事件
                        key_head_handle[i].scan_status = RepeatStatus; //进入重复状态
                    }
                }
            }
            else
            {
                key_head_handle[i].event.click_up = 1;     //点击 松手有效
                key_head_handle[i].scan_status = DEBOUNCE; //标记进入等待状态
            }
            break;

        case RepeatStatus:                      //重复状态
            if (key_head_handle[i].is_key_down) //判断是否按下
            {
                if (key_head_handle[i].repeat_speed > 0) //重复计时>0
                {
                    if (++key_head_handle[i].repeat_count >= key_head_handle[i].repeat_speed) //重复触发计时大于设定值
                    {
                        key_head_handle[i].repeat_count = 0; //重复触发计时清零
#if KEY_MODE
                        if (key_head_handle[i].event.valid) //事件是否有效
                        {
                            key_head_handle[i].event.key_repeat = 1; //标记重复事件
                        }
#else
                        key_head_handle[i].event.key_repeat = 1; //标记重复事件
#endif
                    }
                }
            }
            else
            {
                key_head_handle[i].scan_status = DEBOUNCE; //设为等待状态
            }
            break;

        default:
            key_head_handle[i].scan_status = DEBOUNCE; //设为等待状态
            break;
        }
    }
}
