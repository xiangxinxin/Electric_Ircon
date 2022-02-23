#ifndef __OLED_H
#define __OLED_H
#include "config.h"
#include "stdlib.h"
#include "stm32f0xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//OLED模式设置
//0: 4线串行模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
//2: SPI模式
//3: IIC
#define OLED_MODE 2

//-----------------OLED端口定义----------------
#define OLED_CS P02 //
#define OLED_DC P00 //
#define OLED_WR P46 //
#define OLED_RD P45 //

#define OLED_RST P01

//P20~7,作为数据线
#define DATAOUT(x) P2 = x //输出

//使用4线串行接口时使用
#define OLED_SCLK P15
#define OLED_SDIN P13

#define OLED_CMD 0  //写命令
#define OLED_DATA 1 //写数据

#define USE_ShowNum 0
#define USE_DrawFont16 0
#define USE_DrawFont24 0
#define USE_DrawFont32 0

#define RESET_OLED_CS HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)
#define SET_OLED_CS HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)
#define RESET_OLED_DC HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define SET_OLED_DC HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define RESET_OLED_RST HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define SET_OLED_RST HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)

//HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

//OLED控制用函数
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);

void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size);
#endif
