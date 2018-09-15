#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.7
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 

//0,��֧��ucos
//1,֧��ucos
#define SYSTEM_SUPPORT_OS		0		//����ϵͳ�ļ����Ƿ�֧��UCOS
																	    
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ


//10����תBCD
#define DEC_TO_BCD(dec)                 ((((dec) / 10) << 4) + ((dec) % 10))
// BCDת10����
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
