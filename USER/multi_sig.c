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
** ��������: M U L T I _ S I G _ I N I T
** ��������:
** �������:
** �������:
** �� �� ֵ:
** ��    ��:
** ��    ��:
** ��    ��:
** �޸�����  �汾��   �޸���  �޸�����
**************************************************************/
void multi_sig_init(void)
{
    uint8_t i;
    for (i = 0; i < SIG_NUM; i++) //ɨ����ÿ������
    {
        sig_array[i].is_sig_down = 0;
        sig_array[i].filter_conut = 0;
        sig_array[i].scan_status = PRESS_UP; //
        sig_array[i].event.sig_down = 0;
        sig_array[i].event.sig_up = 1;
    }
}

/**
 * @description: ɨ��
 * @param {type}
 * @return:
 */
void sig_scan(void)
{
    uint8_t i;
    for (i = 0; i < SIG_NUM; i++) //ɨ����ÿ������
    {
        switch (sig_array[i].scan_status)
        {
        case PRESS_UP: //
        {
            if (sig_array[i].is_sig_down) //�жϵײ�������Ƿ��а������� �жϵ���sig_value�е�����
            {
                sig_array[i].filter_conut++;
                if (sig_array[i].filter_conut >= SIG_FILTER_TIME) //�������¼���>=�����˲������������˲���
                {
                    sig_array[i].filter_conut = 0;         //���㰴������ɨ���������
                    sig_array[i].event.sig_rising = 0;     //������
                    sig_array[i].event.sig_falling = 1;    //�½���
                    sig_array[i].scan_status = PRESS_DOWN; //
                }
            }
            else
            {
                sig_array[i].filter_conut = 0; //���㰴������ɨ���������
                // sig_array[i].event.sig_up = 1;   //��ǰ����ɿ����
                // sig_array[i].event.sig_down = 0; //����������±��
            }
            break;
        }

        case PRESS_DOWN: //����ѹ��״̬
        {
            if (sig_array[i].is_sig_down) //�жϵײ�������Ƿ��а�������
            {
                sig_array[i].filter_conut = 0; //���㰴������ɨ���������
            }
            else
            {
                // sig_array[i].event.sig_up = 0;   //������ֱ�־
                // sig_array[i].event.sig_down = 1; //��ǰ��������¼���������

                sig_array[i].filter_conut++;
                if (sig_array[i].filter_conut >= SIG_FILTER_TIME) //�������¼���>=�����˲������������˲���
                {
                    sig_array[i].filter_conut = 0;       //���㰴������ɨ���������
                    sig_array[i].event.sig_rising = 1;   //������
                    sig_array[i].event.sig_falling = 0;  //�½���
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
