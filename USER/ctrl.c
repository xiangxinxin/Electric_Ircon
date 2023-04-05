
#include "ctrl.h"

#define TRMP_LIST_MAX 10

_PWM_T pwm1 = {0};
_CTRL_T ctrl = {0};
uint16_t temperature_list[TRMP_LIST_MAX] = {0}; // t12 热电偶分度表

uint32_t volatile time_cnt_inc = 0; //自定义一个系统架构用的变量
uint8_t uart_temp[128] = {0};

void system_loop(void);

void Init(void) //初始化
{
    //初始化SSD1306
    OLED_Init();

    //  ADCInit();
    // #if 0
    // 	PWM_init();
    // #endif
    multi_key_init();
    multi_sig_init();

    //     FFT_init(); //初始化FFT
}

/******************** 主函数 **************************/
void ctrl_main(void)
{
    Init();     //初始化	
    PID_init(); // PID参数初始化

    // PrintString1("上电初始化完成\r\n");
    while (1)
    {
        if (time_cnt_inc > 0) // 1ms
        {
            if (time_cnt_inc >= 250)
            {
                time_cnt_inc = 250;
            }
            time_cnt_inc--;
            system_loop();
        }
    }
}

void system_loop(void)
{
    static uint8_t loop_cnt = 0x00;
    if (loop_cnt % 5 == 0)
    {
    }
    if (loop_cnt == 11)
    {
    }
    else if (loop_cnt == 33)
    {
        ADC_Process();
    }
    else if (loop_cnt == 44)
    {
    }
    else if (loop_cnt == 55)
    {
        heat_Process(); //加热进程
    }
    else if (loop_cnt == 66)
    {
        display_Process();
    }
    else if (loop_cnt == 77)
    {
        // FFT_Process(); //FFT进程
    }
    else if (loop_cnt == 99)
    {
        // uart_data_process(); //串口数据处理
        // OLED_ShowChar(5, 6, '3', 16, 1);
        OLED_Refresh_Gram(); //更新显示
    }

    if (++loop_cnt >= 100)
    {
        loop_cnt = 0; //表示计数到最大数为
    }
}

uint16_t lookup_temp_list(uint16_t val) //查分度表
{
    uint8_t i, temp;
    uint16_t temperature;
    //二分法查表
    temp = TRMP_LIST_MAX / 2;
    for (i = 0; i < TRMP_LIST_MAX; i++)
    {
        if (val > temperature_list[temp])
        {
            if (val < temperature_list[temp + 1] || temp >= TRMP_LIST_MAX)
            {
                break;
            }
            else
                temp += temp / 2;
        }
        else if (val < temperature_list[temp])
        {
            if (val > temperature_list[temp - 1] || temp <= 0)
                break;
            else
                temp -= temp / 2;
        }
    }
    temperature = temp * 50 + ((temperature_list[temp + 1] - temperature_list[temp]) / (val - temperature_list[temp]));
    return temperature;
}

int ADCRead(uint8_t channel)
{
    int res;
    // if (channel > 0x0F)
    //     return 0;

    // ADC_CONTR &= 0xF0; //使能ADC选择通道
    // ADC_CONTR |= channel;
    // ADC_CONTR |= 0x40;
    // _nop_();
    // _nop_();
    // _nop_();
    // _nop_();
    // while (!(ADC_CONTR & 0x20))
    //     ;                            //查询ADC完成标志
    // ADC_CONTR &= ~0x20;              //清完成标志
    // res = (ADC_RES << 8) | ADC_RESL; //读取ADC结果
    return res;
}

void PWM_irq(void) //中断中处理PWM
{
    if (pwm1.enable) //使能
    {
        if (++pwm1.period_cnt >= pwm1.period)
        {
            pwm1.period_cnt = 0;
        }
        // if (pwm1.period_cnt < pwm1.duty_cycle)
        // {
        //     PWM_IO = 0;
        // }
        // else
        // {
        //     PWM_IO = 1;
        // }
    }
    else
    {
        pwm1.period_cnt = 0;
        // PWM_IO = 1;
    }
}

void SetPWM(uint16_t pwm_val)
{
    pwm1.duty_cycle = (uint8_t)pwm_val;
}

void ADC_Process(void)
{
    uint16_t temp;
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    HAL_Delay(6);

    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 100);
    ctrl.t12_voltage = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 100);
    ctrl.vbus_voltage = HAL_ADC_GetValue(&hadc);
    // ctrl.t12_voltage = (temp + ctrl.t12_voltage) / 2;
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    // HAL_ADC_Start(&hadc);
    // HAL_ADC_PollForConversion(&hadc, 100);
    ctrl.vbus_voltage = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);
}

void heat_Process(void) //加热进程
{
    int16_t temp = 0xFFFF;

    ctrl.t12_set_temperature = ctrl.encoder_cnt * 20; // T12电烙铁设定温度
                                                      //设置占空比
    temp = PID_Operation((float)ctrl.t12_set_temperature, ctrl.t12_voltage);
    if (temp < 0)
        temp = 0;
    else if (temp > 101)
        temp = 101;
    ctrl.pwm_val = (uint16_t)temp;

    TIM3->CCR1 = ctrl.pwm_val; //设置占空比
}

void display_Process(void)
{

    char Dis_temp[30] = "{0}";
    // uint8_t i = 0;

    ctrl.vbus_voltage = 315.0 * (double)ctrl.vbus_voltage / 4095.0;
    ctrl.vbus_voltage *= 11;
    sprintf(Dis_temp, "VBUS V:%4d", (int)ctrl.vbus_voltage);
    OLED_ShowString(1, 0, (uint8_t *)Dis_temp, 12);
    sprintf(Dis_temp, "T12 V:%4d", (int)ctrl.t12_voltage);
    OLED_ShowString(1, 11, (uint8_t *)Dis_temp, 12);
    sprintf(Dis_temp, "set:%4d", (int)ctrl.t12_set_temperature);
    OLED_ShowString(1, 22, (uint8_t *)Dis_temp, 12);
    sprintf(Dis_temp, "pwm:%4d  Pi:%5d", (int)ctrl.pwm_val, (int)pid.integral);
    OLED_ShowString(1, 33, (uint8_t *)Dis_temp, 12);

    OLED_Fill(0, 45, 128, 64, 0);

    OLED_Fill(0, 46, (127 * ctrl.pwm_val / 100), 50, 1);

    if (ctrl.t12_set_temperature)
    {
        if (ctrl.t12_set_temperature < 2)
            OLED_Fill(0 - 2, 52, (127 * ctrl.t12_set_temperature / 4095), 64, 1);
        else if (ctrl.t12_set_temperature < 4095 - 2)
            OLED_Fill((127 * ctrl.t12_set_temperature / 4095) - 2, 52, (127 * ctrl.t12_set_temperature / 4095), 64, 1);
        else
            OLED_Fill(127 - 2, 52, (127 * ctrl.t12_set_temperature / 4095), 64, 1);
    }

    OLED_Fill(0, 54, (127 * ctrl.t12_voltage / 4095), 62, 1);

    // for (i = 0; i <= 127; i++)
    // {
    //     // x1,y1,x2,y2 填充区域的对角坐标
    //     //确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
    //     // dot:0,清空;1,填充
    //     //  void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    //     //  if (FFT_out[i] > 6)
    //     //  {
    //     //      temp = 60;
    //     //  }
    //     //  else
    //     //  {
    //     //      temp = (uint8_t)(FFT_out[i] * 10);
    //     //  }
    // }
}

void sig_in(void)
{
    if (HAL_GPIO_ReadPin(ECD_A_GPIO_Port, ECD_A_Pin) == GPIO_PIN_SET)
        sigA.is_sig_down = 0;
    else
        sigA.is_sig_down = 1;
    if (HAL_GPIO_ReadPin(ECD_B_GPIO_Port, ECD_B_Pin) == GPIO_PIN_SET)
        sigB.is_sig_down = 0;
    else
        sigB.is_sig_down = 1;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t cnt = 0;

    if (htim->Instance == TIM2)
    {
        cnt++;
        if (cnt >= 10)
        {
            cnt = 0;
            time_cnt_inc++; //自定义一个系统架构用的变量
        }
        {
            uint8_t temp;
            sig_in();
            sig_scan();
            temp = encoder_loop(); // return: 0:无变化  1：正转1步  2：反转1步
            if (temp == 1)
            {
                ctrl.encoder_cnt--;
            }
            else if (temp == 2)
            {
                ctrl.encoder_cnt++;
            }
            if (ctrl.encoder_cnt > 200)
                ctrl.encoder_cnt = 200;
            else if (ctrl.encoder_cnt < 0)
                ctrl.encoder_cnt = 0;

            Sys_Scan();
            key_scan();
            key_process();
        }
    }
}
