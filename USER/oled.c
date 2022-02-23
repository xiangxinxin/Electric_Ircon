#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
// #include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//SSD1306 OLED ����IC��������
//������ʽ:8080����/4�ߴ���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/03/07
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.1 20140307
//1,�޸�OLED_ShowChar����,֧��12/16/24����
//2,�޸�OLED_ShowString����,֧����������
//////////////////////////////////////////////////////////////////////////////////

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
uint8_t OLED_GRAM[128][8] = {0};

//�����Դ浽LCD
void OLED_Refresh_Gram(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
		OLED_WR_Byte(0x00, OLED_CMD);	  //������ʾλ�á��е͵�ַ
		OLED_WR_Byte(0x10, OLED_CMD);	  //������ʾλ�á��иߵ�ַ

#if OLED_MODE == 1
		//SPI�Ż�
		if (cmd)
			SET_OLED_DC;
		else
			RESET_OLED_DC;
		;						 //д����
		SPDAT = OLED_GRAM[0][i]; //���Ͳ�������
		for (n = 1; n < 128; n++)
		{
			while (!(SPSTAT & 0x80))
				;					 //��ѯ��ɱ�־
			SPSTAT = 0xc0;			 //���жϱ�־
			SPDAT = OLED_GRAM[n][i]; //���Ͳ�������
		}
#else
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
#endif
	}
}
#if OLED_MODE == 1
#if 0
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//��SSD1306д��һ���ֽڡ� ���ݷ���
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	uint8_t i;
	OLED_DC = cmd; //д����
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

//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
	// /*�о�԰SPI����*/
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
	/*ģ��SPI*/
	{
		uint8_t i;
		OLED_WR = 0;   //д
		OLED_DC = cmd; //д����
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
	/*Ӳ��SPI*/
	{
#define SS OLED_CS
		// SS = 0;		   //���ʹӻ�SS�ܽ�
		OLED_DC = cmd; //д����
		SPCTL = 0x50;  //ʹ��SPI����ģʽ
		SPDAT = dat;   //���Ͳ�������
		while (!(SPSTAT & 0x80))
			;		   //��ѯ��ɱ�־
		SPSTAT = 0xc0; //���жϱ�־
					   // SS = 1;		   //���ߴӻ���SS�ܽ�
	}
#endif
	/*Ӳ��SPI*/
	{
		RESET_OLED_CS;
		// SS = 0;		   //���ʹӻ�SS�ܽ�
		// OLED_DC = cmd; //д����
		if (cmd)
			SET_OLED_DC;
		else
			RESET_OLED_DC;
		HAL_SPI_Transmit(&hspi1, &dat, 1, 1000); //����ʽ��������
		SET_OLED_DC;

		// SPCTL = 0x50; //ʹ��SPI����ģʽ
		// SPDAT = dat;  //���Ͳ�������
		// while (!(SPSTAT & 0x80))
		// 	;		   //��ѯ��ɱ�־
		// SPSTAT = 0xc0; //���жϱ�־
	}
}
#endif

#if OLED_MODE == 3 //IIC
//��ʱ
void IIC_delay(void)
{
	u8 t = 3;
	while (t--)
		;
}

//��ʼ�ź�
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

//�����ź�
void I2C_Stop(void)
{
	OLED_SDIN = 0;
	OLED_SCLK = 1;
	IIC_delay();
	OLED_SDIN = 1;
}

//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
	OLED_SDIN = 1;
	IIC_delay();
	OLED_SCLK = 1;
	IIC_delay();
	OLED_SCLK = 0;
	IIC_delay();
}

//д��һ���ֽ�
void Send_Byte(u8 dat)
{
	u8 i;
	for (i = 0; i < 8; i++)
	{
		if (dat & 0x80) //��dat��8λ�����λ����д��
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
		OLED_SCLK = 0; //��ʱ���ź�����Ϊ�͵�ƽ
		dat <<= 1;
	}
}

//����һ���ֽ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
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

//����OLED��ʾ
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
	OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
	OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
		for (n = 0; n < 128; n++)
			OLED_GRAM[n][i] = 0X00;
	OLED_Refresh_Gram(); //������ʾ
}
//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
	uint8_t pos, bx, temp = 0;
	if (x > 127 || y > 63)
		return; //������Χ��.
	pos = 7 - y / 8;
	bx = y % 8;
	temp = 1 << (7 - bx);
	if (t)
		OLED_GRAM[x][pos] |= temp;
	else
		OLED_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{
	uint8_t x, y;
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
			OLED_DrawPoint(x, y, dot);
	}
	// OLED_Refresh_Gram(); //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 12/16/24
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr = chr - ' ';												//�õ�ƫ�ƺ��ֵ
	for (t = 0; t < csize; t++)
	{
		if (size == 12)
			temp = asc2_1206[chr][t]; //����1206����
		else if (size == 16)
			temp = asc2_1608[chr][t]; //����1608����
		else if (size == 24)
			temp = asc2_2412[chr][t]; //����2412����
		else
			return; //û�е��ֿ�
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
//m^n����
uint32_t mypow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}
//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
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

//��ʾ�ַ���
//x,y:�������
//size:�����С
//*p:�ַ�����ʼ��ַ
void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size)
{
	while ((*p <= '~') && (*p >= ' ') || (*p == '\n')) //�ж��ǲ��ǷǷ��ַ�!
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

//��ʼ��SSD1306
void OLED_Init(void)
{
	// #define GPIO_PullUp 0 //����׼˫���
	// #define GPIO_HighZ 1  //��������
	// #define GPIO_OUT_OD 2 //��©���
	// #define GPIO_OUT_PP 3 //�������

#define PIN_MODE GPIO_OUT_PP
//	GPIO_InitTypeDef GPIO_Init;
#if (OLED_MODE == 0)   //
#elif (OLED_MODE == 1) //ʹ��8080����ģʽ

	GPIO_Init.Mode = GPIO_HighZ;			 //
	GPIO_Init.Pin = GPIO_Pin_3 | GPIO_Pin_5; //
	GPIO_Inilize(GPIO_P1, &GPIO_Init);		 //IO��������  �������

	GPIO_Init.Mode = PIN_MODE;		   //�������
	GPIO_Init.Pin = GPIO_Pin_All;	   //IO��������
	GPIO_Inilize(GPIO_P2, &GPIO_Init); //IO��������  �������

	GPIO_Init.Mode = PIN_MODE;				 //
	GPIO_Init.Pin = GPIO_Pin_5 | GPIO_Pin_6; //
	GPIO_Inilize(GPIO_P4, &GPIO_Init);		 //IO��������  �������

	GPIO_Init.Mode = PIN_MODE;							  //
	GPIO_Init.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; //
	GPIO_Inilize(GPIO_P0, &GPIO_Init);					  //IO��������  �������

#elif (OLED_MODE == 2) //ʹ��4��SPI ����ģʽ
	// GPIO_Init.Mode = PIN_MODE;		   //�������
	// GPIO_Init.Pin = GPIO_Pin_All;	   //IO��������
	// GPIO_Inilize(GPIO_P2, &GPIO_Init); //IO��������  �������
	// P2 = 0x00;

	// GPIO_Init.Mode = GPIO_HighZ;			 //
	// GPIO_Init.Pin = GPIO_Pin_6 | GPIO_Pin_7; //
	// GPIO_Inilize(GPIO_P2, &GPIO_Init);		 //IO��������  �������

	// GPIO_Init.Mode = PIN_MODE;				 //
	// GPIO_Init.Pin = GPIO_Pin_3 | GPIO_Pin_5; //
	// GPIO_Inilize(GPIO_P1, &GPIO_Init);		 //IO��������  �������

	// GPIO_Init.Mode = PIN_MODE;				 //
	// GPIO_Init.Pin = GPIO_Pin_5 | GPIO_Pin_6; //
	// GPIO_Inilize(GPIO_P4, &GPIO_Init);		 //IO��������  �������

	// GPIO_Init.Mode = PIN_MODE;							  //
	// GPIO_Init.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; //
	// GPIO_Inilize(GPIO_P0, &GPIO_Init);					  //IO��������  �������

	// /*Ӳ��SPI*/
	// SPCTL = 0x50;  //ʹ��SPI����ģʽ
	// SPSTAT = 0xc0; //���жϱ�־
	// IE2 = 0;	   //�ر�SPI�ж�
	RESET_OLED_CS;
	//ʹ��SPI�ӻ�

#endif

	RESET_OLED_RST;
	// delay_ms(100);
	HAL_Delay(500);
	SET_OLED_RST;

	OLED_WR_Byte(0xAE, OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5, OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80, OLED_CMD);	  //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8, OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F, OLED_CMD); //Ĭ��0X3F(1/64)
	OLED_WR_Byte(0xD3, OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00, OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40, OLED_CMD); //������ʾ��ʼ�� [5:0],����.

	OLED_WR_Byte(0x8D, OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14, OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20, OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02, OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1, OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0, OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA, OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12, OLED_CMD); //[5:4]����

	OLED_WR_Byte(0x81, OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF, OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9, OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB, OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4, OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6, OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	OLED_WR_Byte(0xAF, OLED_CMD); //������ʾ	OLED_Clear();
}

#if USE_DrawFont16
//******************************************************************
//��������  GUI_DrawFont16
//���ܣ�    ��ʾ����16X16��������
//���������x,y :�������
//			s:�ַ�����ַ
//			mode:ģʽ	0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void GUI_DrawFont16(uint16_t x, uint16_t y, uint8_t *s, uint8_t mode)
{
	uint8_t i, j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0 = x;
	HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); //�Զ�ͳ�ƺ�����Ŀ

	for (k = 0; k < HZnum; k++)
	{
		if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
		{
			// LCD_SetWindows(x, y, x + 16 - 1, y + 16 - 1);
			for (i = 0; i < 16 * 2; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) //�ǵ��ӷ�ʽ
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
							// LCD_DrawPoint(x, y); //��һ����
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
		continue; //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
	// LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); //�ָ�����Ϊȫ��
}
#endif

#if USE_DrawFont24
//******************************************************************
//��������  GUI_DrawFont24
//���ܣ�    ��ʾ����24X24��������
//���������x,y :�������
//			fc:ǰ�û�����ɫ
//			bc:������ɫ
//			s:�ַ�����ַ
//			mode:ģʽ	0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s, uint8_t mode)
{
	uint8_t i, j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0 = x;
	uint16_t colortemp = POINT_COLOR;
	HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); //�Զ�ͳ�ƺ�����Ŀ

	for (k = 0; k < HZnum; k++)
	{
		if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
		{
			LCD_SetWindows(x, y, x + 24 - 1, y + 24 - 1);
			for (i = 0; i < 24 * 3; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) //�ǵ��ӷ�ʽ
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
							LCD_DrawPoint(x, y); //��һ����
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
		continue; //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
	POINT_COLOR = colortemp;
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); //�ָ�����Ϊȫ��
}
#endif

#if USE_DrawFont32
//******************************************************************
//��������  GUI_DrawFont32
//���ܣ�    ��ʾ����32X32��������
//���������x,y :�������
//			fc:ǰ�û�����ɫ
//			bc:������ɫ
//			s:�ַ�����ַ
//			mode:ģʽ	0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s, uint8_t mode)
{
	uint8_t i, j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0 = x;
	uint16_t colortemp = POINT_COLOR;
	HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); //�Զ�ͳ�ƺ�����Ŀ
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
		{
			LCD_SetWindows(x, y, x + 32 - 1, y + 32 - 1);
			for (i = 0; i < 32 * 4; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) //�ǵ��ӷ�ʽ
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
							LCD_DrawPoint(x, y); //��һ����
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
		continue; //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
	POINT_COLOR = colortemp;
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); //�ָ�����Ϊȫ��
}
#endif
