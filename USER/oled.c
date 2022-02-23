#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
// #include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/03/07
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.1 20140307
//1,修改OLED_ShowChar函数,支持12/16/24字体
//2,修改OLED_ShowString函数,支持字体设置
//////////////////////////////////////////////////////////////////////////////////

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
uint8_t OLED_GRAM[128][8] = {0};

//更新显存到LCD
void OLED_Refresh_Gram(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  //设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  //设置显示位置—列高地址

#if OLED_MODE == 1
		//SPI优化
		if (cmd)
			SET_OLED_DC;
		else
			RESET_OLED_DC;
		;						 //写命令
		SPDAT = OLED_GRAM[0][i]; //发送测试数据
		for (n = 1; n < 128; n++)
		{
			while (!(SPSTAT & 0x80))
				;					 //查询完成标志
			SPSTAT = 0xc0;			 //清中断标志
			SPDAT = OLED_GRAM[n][i]; //发送测试数据
		}
#else
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
#endif
	}
}
#if OLED_MODE == 1
#if 0
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	DATAOUT(dat);
	OLED_DC = cmd;
	RESET_OLED_CS;;
	OLED_WR = 0;
	// NOP10();
	OLED_WR = 1;
	SET_OLED_CS;
	if (cmd)
			SET_OLED_DC;
		else
			RESET_OLED_DC; ;
}

#else
//向SSD1306写入一个字节。 数据反向
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	// uint8_t temp, i;
	// temp = 0;
	// OLED_RD = 1;
	// for (i = 0; i < 8; i++)
	// {
	// 	if ((dat >> i) & 0x01)
	// 		temp |= 0x01;
	// 	temp <<= 1;
	// }
	// DATAOUT(temp);
	// OLED_DC = cmd;
	// NOP1();
	// RESET_OLED_CS;;
	// NOP1();
	// OLED_WR = 0;
	// NOP1();
	// OLED_WR = 1;
	// NOP1();
	// SET_OLED_CS;
	// // if (cmd)
	SET_OLED_DC;
	else RESET_OLED_DC;
	;

	uint8_t temp, i;
	temp = 0;
	OLED_RD = 1;
	RESET_OLED_CS;
	;
	NOP1();
	OLED_DC = cmd;
	NOP1();
	for (i = 0; i < 8; i++)
	{
		if ((dat >> i) & 0x01)
			temp |= 0x01;
		temp <<= 1;
	}
	DATAOUT(temp);
	OLED_WR = 0;
	NOP10();
	OLED_WR = 1;
	// NOP1();
	// SET_OLED_CS;
	// if (cmd)
	SET_OLED_DC;
	else RESET_OLED_DC;
	;
}
#endif
#endif

#if OLED_MODE == 0
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	uint8_t i;
	OLED_DC = cmd; //写命令
	RESET_OLED_CS;
	;
	for (i = 0; i < 8; i++)
	{
		OLED_SCLK = 0;
		if (dat & 0x80)
			OLED_SDIN = 1;
		else
			OLED_SDIN = 0;
		OLED_SCLK = 1;
		dat <<= 1;
	}
	SET_OLED_CS;
	if (cmd)
		SET_OLED_DC;
	else
		RESET_OLED_DC;
	;
}
#endif

#if OLED_MODE == 2 //spi

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{

	// 	DATAOUT(dat);
	// 	OLED_DC = cmd;
	// 	RESET_OLED_CS;;
	// 	OLED_WR = 0;
	// 	// NOP10();
	// 	OLED_WR = 1;
	// 	SET_OLED_CS;
	// 	if (cmd)
	// 	SET_OLED_DC;
	// else
	// 	RESET_OLED_DC;
	// }
	// /*中景园SPI例程*/
	// u8 i;
	// if(cmd)
	//   OLED_DC_Set();
	// else
	//   OLED_DC_Clr();
	// OLED_CS_Clr();
	// for(i=0;i<8;i++)
	// {
	// 	OLED_SCLK= 0;
	// 	if(dat&0x80)
	// 	   OLED_SDIN = 1;
	// 	else
	// 	   OLED_SDIN= 0;
	// 	OLED_SCLK= 1;
	// 	dat<<=1;
	// }
	// OLED_CS_Set();
	// OLED_DC_Set();

#if 0
	/*模拟SPI*/
	{
		uint8_t i;
		OLED_WR = 0;   //写
		OLED_DC = cmd; //写命令
		RESET_OLED_CS;;
		for (i = 0; i < 8; i++)
		{
			OLED_SCLK = 0;
			if (dat & 0x80)
				OLED_SDIN = 1;
			else
				OLED_SDIN = 0;
			OLED_SCLK = 1;
			dat <<= 1;
		}
		SET_OLED_CS;
		if (cmd)
			SET_OLED_DC;
		else
			RESET_OLED_DC; ;
	}

#endif

#if 0
	/*硬件SPI*/
	{
#define SS OLED_CS
		// SS = 0;		   //拉低从机SS管脚
		OLED_DC = cmd; //写命令
		SPCTL = 0x50;  //使能SPI主机模式
		SPDAT = dat;   //发送测试数据
		while (!(SPSTAT & 0x80))
			;		   //查询完成标志
		SPSTAT = 0xc0; //清中断标志
					   // SS = 1;		   //拉高从机的SS管脚
	}
#endif
	/*硬件SPI*/
	{
		RESET_OLED_CS;
		// SS = 0;		   //拉低从机SS管脚
		// OLED_DC = cmd; //写命令
		if (cmd)
			SET_OLED_DC;
		else
			RESET_OLED_DC;
		HAL_SPI_Transmit(&hspi1, &dat, 1, 1000); //阻塞式发送数据
		SET_OLED_DC;

		// SPCTL = 0x50; //使能SPI主机模式
		// SPDAT = dat;  //发送测试数据
		// while (!(SPSTAT & 0x80))
		// 	;		   //查询完成标志
		// SPSTAT = 0xc0; //清中断标志
	}
}
#endif

#if OLED_MODE == 3 //IIC
//延时
void IIC_delay(void)
{
	u8 t = 3;
	while (t--)
		;
}

//起始信号
void I2C_Start(void)
{
	OLED_SDIN = 1;
	OLED_SCLK = 1;
	IIC_delay();
	OLED_SDIN = 0;
	IIC_delay();
	OLED_SCLK = 0;
	IIC_delay();
}

//结束信号
void I2C_Stop(void)
{
	OLED_SDIN = 0;
	OLED_SCLK = 1;
	IIC_delay();
	OLED_SDIN = 1;
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	OLED_SDIN = 1;
	IIC_delay();
	OLED_SCLK = 1;
	IIC_delay();
	OLED_SCLK = 0;
	IIC_delay();
}

//写入一个字节
void Send_Byte(u8 dat)
{
	u8 i;
	for (i = 0; i < 8; i++)
	{
		if (dat & 0x80) //将dat的8位从最高位依次写入
		{
			OLED_SDIN = 1;
		}
		else
		{
			OLED_SDIN = 0;
		}
		IIC_delay();
		OLED_SCLK = 1;
		IIC_delay();
		OLED_SCLK = 0; //将时钟信号设置为低电平
		dat <<= 1;
	}
}

//发送一个字节
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat, u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if (mode)
	{
		Send_Byte(0x40);
	}
	else
	{
		Send_Byte(0x00);
	}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}
#endif

//开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
		for (n = 0; n < 128; n++)
			OLED_GRAM[n][i] = 0X00;
	OLED_Refresh_Gram(); //更新显示
}
//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
	uint8_t pos, bx, temp = 0;
	if (x > 127 || y > 63)
		return; //超出范围了.
	pos = 7 - y / 8;
	bx = y % 8;
	temp = 1 << (7 - bx);
	if (t)
		OLED_GRAM[x][pos] |= temp;
	else
		OLED_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{
	uint8_t x, y;
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
			OLED_DrawPoint(x, y, dot);
	}
	// OLED_Refresh_Gram(); //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //得到字体一个字符对应点阵集所占的字节数
	chr = chr - ' ';												//得到偏移后的值
	for (t = 0; t < csize; t++)
	{
		if (size == 12)
			temp = asc2_1206[chr][t]; //调用1206字体
		else if (size == 16)
			temp = asc2_1608[chr][t]; //调用1608字体
		else if (size == 24)
			temp = asc2_2412[chr][t]; //调用2412字体
		else
			return; //没有的字库
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				OLED_DrawPoint(x, y, mode);
			else
				OLED_DrawPoint(x, y, !mode);
			temp <<= 1;
			y++;
			if ((y - y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

#if USE_ShowNum
//m^n函数
uint32_t mypow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (size / 2) * t, y, ' ', size, 1);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size, 1);
	}
}
#endif

//显示字符串
//x,y:起点坐标
//size:字体大小
//*p:字符串起始地址
void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size)
{
	while ((*p <= '~') && (*p >= ' ') || (*p == '\n')) //判断是不是非法字符!
	{
		if ((x > (128 - (size / 2))) || (*p == '\n'))
		{
			x = 0;
			y += size;
		}

		if (*p != '\n')
		{
			if (y > (64 - size))
			{
				return;
				// y = x = 0;
				// OLED_Clear();
			}
			OLED_ShowChar(x, y, *p, size, 1);
			x += size / 2;
		}
		p++;
	}
}

//初始化SSD1306
void OLED_Init(void)
{
	// #define GPIO_PullUp 0 //上拉准双向口
	// #define GPIO_HighZ 1  //浮空输入
	// #define GPIO_OUT_OD 2 //开漏输出
	// #define GPIO_OUT_PP 3 //推挽输出

#define PIN_MODE GPIO_OUT_PP
//	GPIO_InitTypeDef GPIO_Init;
#if (OLED_MODE == 0)   //
#elif (OLED_MODE == 1) //使用8080并口模式

	GPIO_Init.Mode = GPIO_HighZ;			 //
	GPIO_Init.Pin = GPIO_Pin_3 | GPIO_Pin_5; //
	GPIO_Inilize(GPIO_P1, &GPIO_Init);		 //IO所有引脚  推挽输出

	GPIO_Init.Mode = PIN_MODE;		   //推挽输出
	GPIO_Init.Pin = GPIO_Pin_All;	   //IO所有引脚
	GPIO_Inilize(GPIO_P2, &GPIO_Init); //IO所有引脚  推挽输出

	GPIO_Init.Mode = PIN_MODE;				 //
	GPIO_Init.Pin = GPIO_Pin_5 | GPIO_Pin_6; //
	GPIO_Inilize(GPIO_P4, &GPIO_Init);		 //IO所有引脚  推挽输出

	GPIO_Init.Mode = PIN_MODE;							  //
	GPIO_Init.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; //
	GPIO_Inilize(GPIO_P0, &GPIO_Init);					  //IO所有引脚  推挽输出

#elif (OLED_MODE == 2) //使用4线SPI 串口模式
	// GPIO_Init.Mode = PIN_MODE;		   //推挽输出
	// GPIO_Init.Pin = GPIO_Pin_All;	   //IO所有引脚
	// GPIO_Inilize(GPIO_P2, &GPIO_Init); //IO所有引脚  推挽输出
	// P2 = 0x00;

	// GPIO_Init.Mode = GPIO_HighZ;			 //
	// GPIO_Init.Pin = GPIO_Pin_6 | GPIO_Pin_7; //
	// GPIO_Inilize(GPIO_P2, &GPIO_Init);		 //IO所有引脚  推挽输出

	// GPIO_Init.Mode = PIN_MODE;				 //
	// GPIO_Init.Pin = GPIO_Pin_3 | GPIO_Pin_5; //
	// GPIO_Inilize(GPIO_P1, &GPIO_Init);		 //IO所有引脚  推挽输出

	// GPIO_Init.Mode = PIN_MODE;				 //
	// GPIO_Init.Pin = GPIO_Pin_5 | GPIO_Pin_6; //
	// GPIO_Inilize(GPIO_P4, &GPIO_Init);		 //IO所有引脚  推挽输出

	// GPIO_Init.Mode = PIN_MODE;							  //
	// GPIO_Init.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; //
	// GPIO_Inilize(GPIO_P0, &GPIO_Init);					  //IO所有引脚  推挽输出

	// /*硬件SPI*/
	// SPCTL = 0x50;  //使能SPI主机模式
	// SPSTAT = 0xc0; //清中断标志
	// IE2 = 0;	   //关闭SPI中断
	RESET_OLED_CS;
	//使能SPI从机

#endif

	RESET_OLED_RST;
	// delay_ms(100);
	HAL_Delay(500);
	SET_OLED_RST;

	OLED_WR_Byte(0xAE, OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5, OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80, OLED_CMD);	  //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8, OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F, OLED_CMD); //默认0X3F(1/64)
	OLED_WR_Byte(0xD3, OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00, OLED_CMD); //默认为0

	OLED_WR_Byte(0x40, OLED_CMD); //设置显示开始行 [5:0],行数.

	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14, OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20, OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02, OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1, OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0, OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA, OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12, OLED_CMD); //[5:4]配置

	OLED_WR_Byte(0x81, OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF, OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9, OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB, OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4, OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6, OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
	OLED_WR_Byte(0xAF, OLED_CMD); //开启显示	OLED_Clear();
}

#if USE_DrawFont16
//******************************************************************
//函数名：  GUI_DrawFont16
//功能：    显示单个16X16中文字体
//输入参数：x,y :起点坐标
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont16(uint16_t x, uint16_t y, uint8_t *s, uint8_t mode)
{
	uint8_t i, j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0 = x;
	HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); //自动统计汉字数目

	for (k = 0; k < HZnum; k++)
	{
		if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
		{
			// LCD_SetWindows(x, y, x + 16 - 1, y + 16 - 1);
			for (i = 0; i < 16 * 2; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) //非叠加方式
					{
						if (tfont16[k].Msk[i] & (0x80 >> j))
							// LCD_WR_DATA_16Bit(fc);
							OLED_DrawPoint(x, y, 1);
						else
							// LCD_WR_DATA_16Bit(bc);
							OLED_DrawPoint(x, y, 0);
					}
					else
					{
						if (tfont16[k].Msk[i] & (0x80 >> j))
							// LCD_DrawPoint(x, y); //画一个点
							OLED_DrawPoint(x, y, 1);
						x++;
						if ((x - x0) == 16)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue; //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
	// LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); //恢复窗口为全屏
}
#endif

#if USE_DrawFont24
//******************************************************************
//函数名：  GUI_DrawFont24
//功能：    显示单个24X24中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s, uint8_t mode)
{
	uint8_t i, j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0 = x;
	uint16_t colortemp = POINT_COLOR;
	HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); //自动统计汉字数目

	for (k = 0; k < HZnum; k++)
	{
		if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
		{
			LCD_SetWindows(x, y, x + 24 - 1, y + 24 - 1);
			for (i = 0; i < 24 * 3; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) //非叠加方式
					{
						if (tfont24[k].Msk[i] & (0x80 >> j))
							LCD_WR_DATA_16Bit(fc);
						else
							LCD_WR_DATA_16Bit(bc);
					}
					else
					{
						POINT_COLOR = fc;
						if (tfont24[k].Msk[i] & (0x80 >> j))
							LCD_DrawPoint(x, y); //画一个点
						x++;
						if ((x - x0) == 24)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue; //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
	POINT_COLOR = colortemp;
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); //恢复窗口为全屏
}
#endif

#if USE_DrawFont32
//******************************************************************
//函数名：  GUI_DrawFont32
//功能：    显示单个32X32中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s, uint8_t mode)
{
	uint8_t i, j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0 = x;
	uint16_t colortemp = POINT_COLOR;
	HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); //自动统计汉字数目
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
		{
			LCD_SetWindows(x, y, x + 32 - 1, y + 32 - 1);
			for (i = 0; i < 32 * 4; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) //非叠加方式
					{
						if (tfont32[k].Msk[i] & (0x80 >> j))
							LCD_WR_DATA_16Bit(fc);
						else
							LCD_WR_DATA_16Bit(bc);
					}
					else
					{
						POINT_COLOR = fc;
						if (tfont32[k].Msk[i] & (0x80 >> j))
							LCD_DrawPoint(x, y); //画一个点
						x++;
						if ((x - x0) == 32)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue; //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
	POINT_COLOR = colortemp;
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); //恢复窗口为全屏
}
#endif
