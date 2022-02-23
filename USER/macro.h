

#ifndef MACRO_H_
#define MACRO_H_
/* includes --------------------------------------------------------------------------------------*/
#include "stddef.h"
//#include "intrins.H"

/* exported typedef ------------------------------------------------------------------------------*/

//typedef bit              bool;
//typedef char             int8_t;
//typedef unsigned char    uint8_t;
//typedef unsigned int     uint16_t;
//typedef int              int16_t;
//typedef unsigned long    uint32_t;
//typedef long    		     int32_t;


/* exported define -------------------------------------------------------------------------------*/	

/* exported macro --------------------------------------------------------------------------------*/
/**
  * @brief 定义常用的位操作
  * @{
  */
#define _BV(n)              (1<<n)                           //!< 移位操作
#define BIT(n)              _BV(n)                           //!< 移位操作
#define BIT_SET(v,n)        do { v |= _BV(n); } while (0)    //!< 对变量置位
#define BIT_CLR(v,n)        do { v &= ~_BV(n); } while (0)   //!< 对变量清位
#define BIT_TOGGLE(v,n)     do { v ^= _BV(n); } while (0)    //!< 反转位
#define BIT_GET(v,n)	    (v & _BV(n))                     //!< 获取状态
#define BIT_TEST( v, n )       ( (v & _BV(n) ) != 0)

#define MAKEWORD( v1, v2 )    ( ((UINT16)(v1) << 8) + (UINT16)(v2) )
#define HIBYTE( v1 )          ( (UINT8)((v1) >> 8) )
#define LOBYTE( v1 )          ( (UINT8)((v1) & 0xFF) )

#define LONGTOBIN(n) ( ((n >> 21) & 0x80) | \
                       ((n >> 18) & 0x40) | \
                       ((n >> 15) & 0x20) | \
                       ((n >> 12) & 0x10) | \
                       ((n >> 9 ) & 0x08) | \
                       ((n >> 6 ) & 0x04) | \
                       ((n >> 3 ) & 0x02) | \
                       ((n      ) & 0x01) )
#define BIN(n)  LONGTOBIN(0x##n##l)   // Write binary charactor set example:BIN(11111111) = 0xFF

#define		BIT0	0x01
#define		BIT1	0x02
#define		BIT2	0x04
#define		BIT3	0x08
#define		BIT4	0x10
#define		BIT5	0x20
#define		BIT6	0x40
#define		BIT7	0x80
#define		BIT8	0x0100
#define		BIT9	0x0200

/* 开关全局中断的宏 */
#define ENABLE_INT()	cli()	/* 使能全局中断 */
#define DISABLE_INT()	sei()	/* 禁止全局中断 */

// /* debug调试 */
// #define DEBUG 1

#ifndef TRUE
	#define TRUE  (1)
#endif

#ifndef FALSE
	#define FALSE (0)
#endif

#ifndef ON
	#define ON  (1)
#endif

#ifndef OFF
	#define OFF (0)
#endif

#ifndef ENABLE
	#define ENABLE  (1)
#endif

#ifndef DISABLE
	#define DISABLE (0)
#endif

#ifndef OPEN
	#define OPEN  (1)
#endif

#ifndef CLOSE
	#define CLOSE (0)
#endif

/**
  * @}
  */

/**
  * @brief 获取数组长度
  * @{
  */
#define ARRAY_SIZE(a)       (sizeof(a)/sizeof(a[0]))        //!< 获取数组长度
/**						    
  * @}			    
  */					    
						    
/**						    
  * @brief 常用段码显示宏	 


  * @{
  */
#define DIG_SEG_A			_BV(7)
#define DIG_SEG_B			_BV(6)
#define DIG_SEG_C			_BV(5)
#define DIG_SEG_D			_BV(4)
#define DIG_SEG_E			_BV(1)
#define DIG_SEG_F			_BV(3)
#define DIG_SEG_G			_BV(2)
#define DIG_SEG_H			_BV(0)

#define DIG_CHAR_0			(DIG_SEG_A|DIG_SEG_B|DIG_SEG_C|DIG_SEG_D|DIG_SEG_E|DIG_SEG_F)
#define DIG_CHAR_1			(DIG_SEG_B|DIG_SEG_C)
#define DIG_CHAR_2			(DIG_SEG_A|DIG_SEG_B|DIG_SEG_G|DIG_SEG_E|DIG_SEG_D)
#define DIG_CHAR_3			(DIG_SEG_A|DIG_SEG_B|DIG_SEG_G|DIG_SEG_C|DIG_SEG_D)
#define DIG_CHAR_4			(DIG_SEG_F|DIG_SEG_G|DIG_SEG_B|DIG_SEG_C)
#define DIG_CHAR_5			(DIG_SEG_F|DIG_SEG_G|DIG_SEG_C|DIG_SEG_D|DIG_SEG_A)
#define DIG_CHAR_6			(DIG_SEG_A|DIG_SEG_F|DIG_SEG_E|DIG_SEG_D|DIG_SEG_C|DIG_SEG_G)
#define DIG_CHAR_7			(DIG_SEG_A|DIG_SEG_B|DIG_SEG_C)
#define DIG_CHAR_8			(DIG_SEG_A|DIG_SEG_B|DIG_SEG_C|DIG_SEG_D|DIG_SEG_E|DIG_SEG_F|DIG_SEG_G)
#define DIG_CHAR_9			(DIG_SEG_A|DIG_SEG_F|DIG_SEG_G|DIG_SEG_B|DIG_SEG_C|DIG_SEG_D)
#define DIG_CHAR_A			(DIG_SEG_F|DIG_SEG_E|DIG_SEG_A|DIG_SEG_B|DIG_SEG_C|DIG_SEG_G)
#define DIG_CHAR_B			(DIG_SEG_F|DIG_SEG_E|DIG_SEG_G|DIG_SEG_C|DIG_SEG_D)
#define DIG_CHAR_C			(DIG_SEG_A|DIG_SEG_F|DIG_SEG_E|DIG_SEG_D)
#define DIG_CHAR_D			(DIG_SEG_G|DIG_SEG_E|DIG_SEG_D|DIG_SEG_B|DIG_SEG_C)
#define DIG_CHAR_E			(DIG_SEG_A|DIG_SEG_F|DIG_SEG_E|DIG_SEG_D|DIG_SEG_G)
#define DIG_CHAR_F			(DIG_SEG_A|DIG_SEG_F|DIG_SEG_E|DIG_SEG_G)
#define DIG_CHAR_L			(DIG_SEG_F|DIG_SEG_E|DIG_SEG_D)
#define DIG_CHAR_U			(DIG_SEG_F|DIG_SEG_E|DIG_SEG_D|DIG_SEG_C|DIG_SEG_B)
#define DIG_CHAR_P			(DIG_SEG_F|DIG_SEG_E|DIG_SEG_A|DIG_SEG_B|DIG_SEG_G)
#define DIG_CHAR_H			(DIG_SEG_F|DIG_SEG_E|DIG_SEG_G|DIG_SEG_B|DIG_SEG_C)
#define DIG_CHAR_h			(DIG_SEG_F|DIG_SEG_E|DIG_SEG_G|DIG_SEG_C)


/**
  * @}
  */

/**
  * @brief 空指令操作
  * @{
  */
#define NOP1()				do{_nop_();}while(0)
#define NOP2()				do{NOP1();NOP1();}while(0)
#define NOP3()				do{NOP2();NOP1();}while(0)
#define NOP4()				do{NOP3();NOP1();}while(0)
#define NOP5()				do{NOP4();NOP1();}while(0)
#define NOP6()				do{NOP5();NOP1();}while(0)
#define NOP7()				do{NOP6();NOP1();}while(0)
#define NOP8()				do{NOP7();NOP1();}while(0)
#define NOP9()				do{NOP8();NOP1();}while(0)
#define NOP10()				do{NOP9();NOP1();}while(0)
#define NOP11()				do{NOP10();NOP1();}while(0)
#define NOP12()				do{NOP11();NOP1();}while(0)
#define NOP13()				do{NOP12();NOP1();}while(0)
#define NOP14()				do{NOP13();NOP1();}while(0)
#define NOP15()				do{NOP14();NOP1();}while(0)
#define NOP16()				do{NOP15();NOP1();}while(0)
#define NOP17()				do{NOP16();NOP1();}while(0)
#define NOP18()				do{NOP17();NOP1();}while(0)
#define NOP19()				do{NOP18();NOP1();}while(0)
#define NOP20()				do{NOP19();NOP1();}while(0)
#define NOP21()				do{NOP20();NOP1();}while(0)
#define NOP22()				do{NOP21();NOP1();}while(0)
#define NOP23()				do{NOP22();NOP1();}while(0)
#define NOP24()				do{NOP23();NOP1();}while(0)
#define NOP25()				do{NOP24();NOP1();}while(0)
#define NOP26()				do{NOP25();NOP1();}while(0)
#define NOP27()				do{NOP26();NOP1();}while(0)
#define NOP28()				do{NOP27();NOP1();}while(0)
#define NOP29()				do{NOP28();NOP1();}while(0)
#define NOP30()				do{NOP29();NOP1();}while(0)
/**
  * @}
  */

/* exported variables ----------------------------------------------------------------------------*/



/* exported function prototypes ------------------------------------------------------------------*/
#endif
/* end of file -----------------------------------------------------------------------------------*/
