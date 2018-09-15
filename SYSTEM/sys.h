#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.7
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持UCOS
																	    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址


//10进制转BCD
#define DEC_TO_BCD(dec)                 ((((dec) / 10) << 4) + ((dec) % 10))
// BCD转10进制
#define BCD_TO_DEC(bcd)                 ((((bcd) >> 4) * 10) + ((bcd) & 0x0f))


void clearbuff(unsigned char * buff,unsigned int len);
unsigned char read_address(void);
int compare_char_buf(unsigned char *a_buf,unsigned char *b_buf,unsigned char len);
unsigned short CRC16(unsigned char *pParaData,unsigned int nLen) ;
	extern unsigned char uchCRCHi,uchCRCLo;

double ctrl_dot(double da,int xsw);
unsigned int change_DEC_to_BCD(double da,unsigned char *b_buff);
double change_BCD_to_DEC(unsigned char *b_buff,int len);
unsigned int change_DEC_to_ASC(double da,unsigned char *a_buff);
unsigned int change_ASC_to_DEC(unsigned char *a_buff,int len);
unsigned char change_DEC_to_HEX(u32 da,unsigned char *h_buff);
float  change_HEX_to_DEC(unsigned char *h_buff,int len);

void char_string_right(unsigned char *s_chr,unsigned int s_len,unsigned char *t_chr,int t_len);
float Hex_To_Decimal(unsigned char *Byte,int num);
double re_sssw(unsigned char da);
double re_zldw(unsigned char da);
unsigned int DOUB_to_Jingque32bit(double da,unsigned char xsw,unsigned char *buf);
double Jingque32bit_to_DOUB(unsigned char *buf);
double u_pow(double num, int power);
unsigned char change_1BCD_to_1DEC(unsigned char dec);
int Bcd2Asc(unsigned char *input, unsigned int inputLen, unsigned char *output);
int change_ASC_FU_to_DEC(unsigned char *a_buff,int len);
u8 pd_time_baohan(u16 bg_h,u8 bg_m,u8 bg_s, u16 ed_h,u8 ed_m, u8 ed_s, u16 nw_h,u8 nw_m, u8 nw_s);
u8 pd_day_baohan(u16 bg_y,u8 bg_m,u8 bg_d, u16 ed_y,u8 ed_m, u8 ed_d, u16 nw_y,u8 nw_m, u8 nw_d);

u8  pd_time(u16  des_h,u8  des_m,u8  des_s,u8  now_h,u8  now_m,u8  now_s);
u8 pd_ymdhms_baohan(u16 bg_y,u8 bg_mon, u8 bg_d, u8 bg_h, u8 bg_min, u8 bg_s,
								u16 ed_y,u8 ed_mon, u8 ed_d, u8 ed_h, u8 ed_min, u8 ed_s,
								u16 nw_y,u8 nw_mon, u8 nw_d, u8 nw_h, u8 nw_min, u8 nw_s);
u8 CS_Check(u8 *data, u16 length);
float Hex_To_Float(unsigned char *Byte,int num);
u8 Detect_Time_YMD(u16  des_y,u8  des_m,u8  des_d,u16  now_y,u8  now_m,u8  now_d);

#endif
