/*
 * @Description:
 * @Author:
 * @Change Logs:
 * @Date            Author        Notes:
 * 2021-4-9      the first version
 */

#include "multi_sig.h"

MULTI_SIG_T sig_array[SIG_NUM] = {0};

/************************************************************
** 函数名称: M U L T I _ S I G _ I N I T
** 功能描述:
** 输入参数:
** 输出参数:
** 返 回 值:
** 作    者:
** 日    期:
** 版    本:
** 修改日期  版本号   修改人  修改内容
**************************************************************/
void multi_sig_init(void)
{
    uint8_t i;
    for (i = 0; i < SIG_NUM; i++) //扫描中每个按键
    {
        sig_array[i].is_sig_down = 0;
        sig_array[i].filter_conut = 0;
        sig_array[i].scan_status = PRESS_UP; //
        sig_array[i].event.sig_down = 0;
        sig_array[i].event.sig_up = 1;
    }
}

/**
 * @description: 扫描
 * @param {type}
 * @return:
 */
void sig_scan(void)
{
    uint8_t i;
    for (i = 0; i < SIG_NUM; i++) //扫描中每个按键
    {
        switch (sig_array[i].scan_status)
        {
        case PRESS_UP: //
        {
            if (sig_array[i].is_sig_down) //判断底层物理层是否有按键按下 判断的是sig_value中的数据
            {
                sig_array[i].filter_conut++;
                if (sig_array[i].filter_conut >= SIG_FILTER_TIME) //按键按下计数>=按键滤波变量（用于滤波）
                {
                    sig_array[i].filter_conut = 0;         //清零按键按下扫描次数计数
                    sig_array[i].event.sig_rising = 0;     //上升沿
                    sig_array[i].event.sig_falling = 1;    //下降沿
                    sig_array[i].scan_status = PRESS_DOWN; //
                }
            }
            else
            {
                sig_array[i].filter_conut = 0; //清零按键按下扫描次数计数
                // sig_array[i].event.sig_up = 1;   //标记按键松开标记
                // sig_array[i].event.sig_down = 0; //清除按键按下标记
            }
            break;
        }

        case PRESS_DOWN: //按键压下状态
        {
            if (sig_array[i].is_sig_down) //判断底层物理层是否有按键按下
            {
                sig_array[i].filter_conut = 0; //清零按键按下扫描次数计数
            }
            else
            {
                // sig_array[i].event.sig_up = 0;   //清除松手标志
                // sig_array[i].event.sig_down = 1; //标记按键按下事件（触发）

                sig_array[i].filter_conut++;
                if (sig_array[i].filter_conut >= SIG_FILTER_TIME) //按键按下计数>=按键滤波变量（用于滤波）
                {
                    sig_array[i].filter_conut = 0;       //清零按键按下扫描次数计数
                    sig_array[i].event.sig_rising = 1;   //上升沿
                    sig_array[i].event.sig_falling = 0;  //下降沿
                    sig_array[i].scan_status = PRESS_UP; //
                }
            }
        }
        break;

        default:
            sig_array[i].scan_status = PRESS_UP; //
            break;
        }
    }
}
