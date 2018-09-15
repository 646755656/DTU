#include "sys.h"
#include <math.h>
#include <stdio.h>

#if 1
u8  auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
                                                              
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,

0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,

0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,

0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,

0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,

0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,

0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,

0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40

} ;
/*---------------------------------------------------------------------------
crc check tabel lo
---------------------------------------------------------------------------*/
u8  auchCRCLo[] = {

0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,

0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,

0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,

0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,

0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,

0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,

0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,

0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,

0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,

0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,	   

0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,

0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,

0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,

0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,

0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,

0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,

0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,

0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,

0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,

0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,

0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,

0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,

0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,

0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,

0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,

0x43, 0x83, 0x41, 0x81, 0x80, 0x40

};
#endif

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

//*********************************
//清串
//*********************************
void clearbuff(unsigned char * buff,unsigned int len)
{
	unsigned int i=0;
	for(i=0;i<len;i++) buff[i] = 0;
}
//*********************************
//比较串
//---------------------------------------------
//param    a_buf     char   串1
//param    b_buf     char   串2
//param    len       char   串长度
//return     0相同  -1不同
//---------------------------------------------
int compare_char_buf(unsigned char *a_buf,unsigned char *b_buf,unsigned char len)
{
	int revd=0;
	unsigned char i=0;
	
	for(i=0;i<len;i++)
	{
		if(a_buf[i] != b_buf[i]){revd = (-1);break;}
	}
	return( revd );
}

//*********************************
//CRC 校验
//*********************************
unsigned char uchCRCHi;
unsigned char uchCRCLo;
#if 0
unsigned short CRC16(unsigned char *pParaData,unsigned int nLen) 
{
	u8 uIndex = 0;    
	uchCRCHi = 0xFF ;    
	uchCRCLo = 0xFF ;   
	while (nLen--)              
	{
		uIndex = uchCRCLo ^ *pParaData++ ;       
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex] ;
		uchCRCHi = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}
#else
unsigned short CRC16(unsigned char *pParaData,unsigned int nLen) 
{
	int i;
	int j;
	unsigned char CH = 0XA0;  
	unsigned char CL = 0X01;  
	unsigned char SaveHi;
	unsigned char SaveLo;

	uchCRCHi = 0xFF;
	uchCRCLo = 0xFF;

	for (i=0;i<nLen;i++)
	{
		//uchCRCLo = uchCRCLo ^ (*(pParaData+i));
		uchCRCLo = uchCRCLo ^ pParaData[i];
		for (j=0;j<8;j++)
		{
			SaveHi = uchCRCHi;
			SaveLo = uchCRCLo;
		   
			uchCRCHi = uchCRCHi >> 1;
			uchCRCLo = uchCRCLo >> 1;
         	
			if ((SaveHi & 0x01 ) == 0x01 )
				uchCRCLo = uchCRCLo | 0x80; 

			if ((SaveLo & 0x01) == 0x01)
			{
				uchCRCHi = uchCRCHi ^ CH; 
				uchCRCLo = uchCRCLo ^ CL; 
			}
		}
	}
	return((uchCRCHi<<8)+uchCRCLo);
}

#endif


//*********************************
//保留小数
//---------------------------------
//da    数值
//xsw  有效小数位
//*********************************
double ctrl_dot(double da,int xsw)
{
        unsigned ls_dz=0;
        double ls_da=0;

	ls_da = da * pow(10,xsw) + 0.5;
	ls_dz = (int)(ls_da);
        //for(i=0;i<xsw;i++)
	  ls_da = ls_dz / pow(10,xsw);

	return( ls_da );
}

//*************************************
//BCD转10进制
//-------------------------------------
//@brief  BCD转DEC
//@param   b_buff   类型  BCD   BCD串存储区
//@param   len      类型  int   BCD串长度
//@return  unsigned int   
//*************************************
double change_BCD_to_DEC(unsigned char *b_buff,int len)
{
	double rez=0;
	int i=0;
//	printf("\n---BCD>>DEC----\n");

	for(i=0;i<len;i++){
		rez = rez + ((int)(b_buff[i]&0xf0)>>4) * u_pow(10,(len*2-i*2)-1);
		rez = rez + ((int)(b_buff[i]&0x0f) * u_pow(10,(len*2-i*2-2)));
	}

	return( rez );
}
//*************************************
//10进制转BCD
//-------------------------------------
//@brief  DEC转BCD
//@param   da       类型  unsigned int  
//@param   b_buff   类型  BCD
//@return  int     BCD串长度   失败0
//*************************************
unsigned int change_DEC_to_BCD(double da,unsigned char *b_buff)
{
	int i=0,len=0;
	unsigned int bitz=0;

	da = da + 0.5;
	
	if(da >= 10000){
		if((da >= 10000) && (da < 100000)) len = 5;
			else if((da >= 100000) && (da < 1000000)) len = 6;
				else if((da >= 1000000) && (da < 10000000)) len = 7;
					else  if((da >= 10000000) && (da < 100000000)) len = 8;
						else  if((da >= 100000000) && (da < 1000000000)) len = 9;
							else  if((da >= 1000000000.0) && (da < 10000000000.0)) len = 10;
								else  if((da >= 10000000000.0) && (da < 100000000000.0)) len = 11;
									else len = 0;
	}
	else{
		if((da < 10000) && (da >= 1000)) len = 4;
			else if((da < 1000) && (da >= 100)) len = 3;
				else if((da < 100) && (da >= 10)) len = 2;
					else len = 1;
	}

	bitz=0;
	if((len%2) == 0){//偶数位
		for(i=0;i<(len/2);i++){
			da = da - bitz * u_pow(10,len-(i*2));
			bitz = (int)(da/u_pow(10,len-(i*2)-1));
			b_buff[i] = (bitz << 4);
			
			da = da - bitz * u_pow(10,len-(i*2)-1);
			bitz = (int)(da/u_pow(10,len-(i*2)-2));
			b_buff[i] |= bitz;
		}	
		len = len / 2;
	}
	else if((len%2) == 1){//奇数位
		bitz = 0;
		bitz = (int)(da/u_pow(10,len-1));
		b_buff[0] = bitz;

		for(i=1;i<((len+1)/2);i++){
			da = da - bitz * u_pow(10,len-(i*2-1));
			bitz = (int)(da/u_pow(10,len-(i*2-1)-1));
			b_buff[i] = (bitz << 4);
			
			da = da - bitz * u_pow(10,len-(i*2-1)-1);
			bitz = (int)(da/u_pow(10,len-(i*2-1)-2));
			b_buff[i] |= bitz;
		}
		len = (len + 1) / 2;
	}
	else len = 0;
	return( len );
}

//*************************************
//ASCII转10进制
//-------------------------------------
//@brief  ASCII转DEC
//@param   a_buff   类型  ASCII   ASCII串存储区
//@param   len      类型  int     ASCII串长度
//@return  unsigned int   
//*************************************
unsigned int change_ASC_to_DEC(unsigned char *a_buff,int len)
{
	unsigned int rez=0;
	int i=0;
	
	//--------------------------------
	//处理空格与TAB
	for(i=0;i<len;i++) if((a_buff[i] == ' ') || (a_buff[i] == '	')) a_buff[i] = 0x30;
	
	for(i=0;i<len;i++)
	{

//printf("%c ",a_buff[i]);
		if((a_buff[i]>='0')&&(a_buff[i]<='9'))
			rez = (int)(a_buff[i]-0x30) * u_pow(10,len-i-1) + rez;
		else if((a_buff[i]>='A')&&(a_buff[i]<='F'))
			rez =(int)(a_buff[i]-0x37) * u_pow(10,len-i-1) + rez; 
		else if((a_buff[i]>='a')&&(a_buff[i]<='f'))
			rez =(int)(a_buff[i]-0x57) * u_pow(10,len-i-1) + rez; 
	}
	return( rez );
}

//*************************************
//ASCII转带符号10进制
//-------------------------------------
//@brief  ASCII转DEC
//@param   a_buff   类型  ASCII   ASCII串存储区
//@param   len      类型  int     ASCII串长度
//@return  unsigned int   
//*************************************
int change_ASC_FU_to_DEC(unsigned char *a_buff,int len)
{
	int rez=0;
	unsigned int rz=0;

	if(a_buff[0] == '-')
	{
		rz = change_ASC_to_DEC(&a_buff[1],(len-1));
		rez = (int)rz * (-1);
	}
	else
	{
		rez = (int)change_ASC_to_DEC(a_buff,len);
	}

	return( rez );
}

//*************************************
//10进制转ASCII
//-------------------------------------
//@brief  DEC转ASCII
//@param   a_buff   类型  ASCII   ASCII串存储区
//@param   len      类型  int     ASCII串长度
//@return  int      转换后ASCII串长度  0失败   
//*************************************
unsigned int change_DEC_to_ASC(double da,unsigned char *a_buff)
{
	int i=0,len=0;
	unsigned int bitz=0;
/*
	if(da >= 10000){
		if((da >= 10000) && (da < 100000)) len = 5;
			else if((da >= 100000) && (da < 1000000)) len = 6;
				else if((da >= 1000000) && (da < 10000000)) len = 7;
					else  if((da >= 10000000) && (da < 100000000)) len = 8;
						else  if((da >= 100000000) && (da < 1000000000)) len = 9;
							else len = -1;
	}
	else{
		if((da < 10000) && (da >= 1000)) len = 4;
			else if((da < 1000) && (da >= 100)) len = 3;
				else if((da < 100) && (da >= 10)) len = 2;
					else len = 1;
	}
*/
	if(da >= 10000){
		if((da >= 10000) && (da < 100000)) len = 5;
			else if((da >= 100000) && (da < 1000000)) len = 6;
				else if((da >= 1000000) && (da < 10000000)) len = 7;
					else  if((da >= 10000000) && (da < 100000000)) len = 8;
						else  if((da >= 100000000) && (da < 1000000000)) len = 9;
							else  if((da >= 1000000000.0) && (da < 10000000000.0)) len = 10;
								else  if((da >= 10000000000.0) && (da < 100000000000.0)) len = 11;
									else len = 0;
	}
	else{
		if((da < 10000) && (da >= 1000)) len = 4;
			else if((da < 1000) && (da >= 100)) len = 3;
				else if((da < 100) && (da >= 10)) len = 2;
					else len = 1;
	}

	bitz = 0;
	for(i=0;i<len;i++){
		da = da - bitz * u_pow(10,len-i);
		bitz = (int)(da/u_pow(10,len-i-1));
		a_buff[i] = bitz + 0x30;
	}
	a_buff[i] = '\0';

	return(len);
}
//***********************************************
// 串左对齐
//-----------------------------------------------
//s_chr    源串
//s_len    源串长度
//t_chr    目标串
//t_len    目标串长度
//***********************************************
void char_string_right(unsigned char *s_chr,unsigned int s_len,unsigned char *t_chr,int t_len)
{
	int i=0,tw=0,sw=0;

	if(s_len == 0)
	{
		for(i=0;i<t_len;i++)
			t_chr[i] = 0;
		return;
	}

//	t_len --;s_len--;
	tw = t_len - 1;
	sw = s_len - 1; 
	for(i=0;i<s_len;i++)
	{
		t_chr[tw --] = s_chr[sw -- ];
	}
	tw = t_len - s_len;
	for(i=0;i<tw;i++)
	{
		t_chr[i] = 0;
	}
}


//*********************************
//HEX 串转double
//*********************************
float  change_HEX_to_DEC(unsigned char *h_buff,int len)
{
	int i=0;
	float rez=0;
	
	for(i=0;i<len;i++)
		rez = rez + (int)h_buff[i] * pow(256,len-i-1);

	return( rez );
}

//*************************************
//  1字节 BCD转 1字节 DEC
//-------------------------------------
unsigned char change_1BCD_to_1DEC(unsigned char dec)
{
	unsigned rz=0;

	if(dec > 0x99) return( 0 );

	if((dec & 0xF0) == 0x10) rz = 10;
	else if((dec & 0xF0) == 0x20) rz = 20;
	else if((dec & 0xF0) == 0x30) rz = 30;
	else if((dec & 0xF0) == 0x40) rz = 40;
	else if((dec & 0xF0) == 0x50) rz = 50;
	else if((dec & 0xF0) == 0x60) rz = 60;
	else if((dec & 0xF0) == 0x70) rz = 70;
	else if((dec & 0xF0) == 0x80) rz = 80;
	else if((dec & 0xF0) == 0x90) rz = 90;

	if((dec & 0x0F) == 0x01) rz += 1;
	else if((dec & 0x0F) == 0x02) rz += 2;
	else if((dec & 0x0F) == 0x03) rz += 3;
	else if((dec & 0x0F) == 0x04) rz += 4;
	else if((dec & 0x0F) == 0x05) rz += 5;
	else if((dec & 0x0F) == 0x06) rz += 6;
	else if((dec & 0x0F) == 0x07) rz += 7;
	else if((dec & 0x0F) == 0x08) rz += 8;
	else if((dec & 0x0F) == 0x09) rz += 9;

	return( rz );
}


//***********************************************
//DEC 转 HEX(先发送高位)
//-----------------------------------------------
unsigned char change_DEC_to_HEX(u32 da,unsigned char *h_buff)
{
	int i=0,len=0;
	unsigned int bitz=0;

	if(da<255) len = 1;
		else if(da<65535) len = 2;
			else if(da<16777215) len = 3;
				else if(da<=4294967294) len = 4;
					else len = 0;

	bitz = 0;				
	if(len != (-1)){
		for(i=0;i<len;i++){
			da = da - bitz * (int)(pow(256,(len-i)));
			bitz = da / (int)(pow(256,(len-i-1)));
			h_buff[i] = bitz;
		}
	}
	
	return( len );
}

//***************************
// 16进制浮点转double
//*************************
float Hex_To_Decimal(unsigned char *Byte,int num)
{
	int i;
	float pfValue;
	char cByte[4]; 	  
	for (i=0;i<num;i++)  
	{  
		cByte[i] = Byte[i];   
	}	   
  	pfValue=*(float*)&cByte; 
	return  pfValue;
}


double re_sssw(unsigned char da)
{
	double rz = 0;
	double fh=0;

	if((da&0x80) == 0x80)		// 负流量
	{
		fh = -1.0;
	}
	else						// 正流量
	{
		fh = 1.0;
	}
	
	da = da >> 4;
	da =  da & 0x07;

	switch( da )
	{
		case 0:
			rz = 100000.0;
			break;
		case 1:
			rz = 10000.0;
			break;
		case 2:
			rz = 1000.0;
			break;
		case 3:
			rz = 100.0;
			break;
		case 4:
			rz = 10.0;
			break;
		case 5:
			rz = 1.0;
			break;
		default: 
			rz = 1.0;
			break;
	}

	rz = rz * fh;
	
	return( rz );
}


double re_zldw(unsigned char da)
{
	double rz=0;

	switch( da )
	{
		case 0:
		case 4:
		case 8:
			rz = 1000.0;
			break;
		case 1:
		case 5:
		case 9:
			rz = 100.0;
			break;
		case 2:
		case 6:
		case  10:
			rz = 10.0;
			break;
		case 3:
		case 7:
		case 11:
			rz = 1.0;
			break;
		default: 
			rz = 1.0;
			break;
	}

	return( rz );
}

//*************************************
//-------------------------------------
double u_pow(double num, int power)
{
	int i=0;
	double rz=1;
	if(power >= 0)
		for(i=0;i<power;i++) rz = rz * num;
	else 
	{
		power = power * (-1);
		for(i=0;i<power;i++) rz = rz *  num;
		rz = 1.0 / rz;
	}
	
	return( rz );
}


//***********************************************
//精确32位小数转浮点
//-----------------------------------------------
//buf [d1, d2, d3, d4, 小数位]
//***********************************************
double Jingque32bit_to_DOUB(unsigned char *buf)
{
	double ra=0;
//printf("\n[%02x][%02x][%02x][%02x][%02x]\n",buf[0],buf[1],buf[2],buf[3],buf[4]);
	ra = u_pow(10,buf[4]);
//printf("\nra = %f\n",ra);	
	ra = change_HEX_to_DEC(buf,4) / ra;
//printf("\nra = %f\n",ra);	
	return( ra );
}



//***********************************************
//浮点转精确32位小数
//-----------------------------------------------
//buf	 [d1, d2, d3, d4, 小数位]
// da 	值
// xsw	小数位
//***********************************************
unsigned int DOUB_to_Jingque32bit(double da,unsigned char xsw,unsigned char *buf)
{
	unsigned char ls_chr[5],sc_chr[5];
	unsigned int l=0;
	double la=0;

	la = pow(10,xsw);

	la = da * la;

	l = change_DEC_to_HEX((unsigned int)la,ls_chr);

	char_string_right(ls_chr,l,sc_chr,4);
	sc_chr[4] = xsw;

	for(l=0;l<5;l++)
	{
		buf[l] = sc_chr[l];
	}

	return( 5 );
}




// ASCII转BCD
int Bcd2Asc(unsigned char *input, unsigned int inputLen, unsigned char *output)

{
    const unsigned char ascii[16] = "0123456789ABCDEF";
    unsigned int i;

    if((input == NULL) || (output == NULL))
		{
        return (ERROR);
    }

    for (i = 0; i < inputLen; i++)
    {
        output[2 * i]   = ascii[(input[i] >> 4)];
        output[2 * i + 1] = ascii[(input[i] & 0x0F)];
    }
    return 0;
}


//***********************************************
//判断当前时间与设定时间的关系
//-----------------------------------------------
//bg_y、bg_m、bg_d    设定时间段开始年时、月分、日秒
//ed_y、ed_m、ed_d    设定时间段结束年时、月分、日秒
//nw_y、nw_m、nw_d    当前年时、月分、日秒
//return  0-不包含 1-包含中  2-等于bg  3-等于ed
//***********************************************
u8 pd_time_baohan(u16 bg_h,u8 bg_m,u8 bg_s, u16 ed_h,u8 ed_m, u8 ed_s, u16 nw_h,u8 nw_m, u8 nw_s)
{
	unsigned char rec=0;

	if((nw_h == bg_h) && (nw_m == bg_m) && (nw_s == bg_s)) rec = 2;
	else if((nw_h == ed_h) && (nw_m == ed_m) && (nw_s == ed_s)) rec = 3;
	else
	{
		if(bg_h == ed_h)//时相等
		{
			if(bg_m == ed_m) //分相等
			{
				if((nw_s > bg_s) && (nw_s < ed_s)) //秒包含
					rec = 1;
				else if((nw_s < bg_s) || (nw_s > ed_s))
					rec  = 0;
			}
			else if((nw_m > bg_m) && (nw_m < ed_m)) //分包含
				rec = 1;
			else if((nw_m < bg_m) || (nw_m > ed_m)) //分不包含
				rec  = 0;
		}
		else//时不等
		{
			if((nw_h < bg_h) || (nw_h > ed_h)) rec = 0;
			else if(nw_h == bg_h)//与起始小时相等
			{
				if(nw_m == bg_m)
				{
					if(nw_s < bg_s)
						rec = 0;
					else
						rec = 1;
				}				
				else if(nw_m < bg_m)
					rec = 0;
				else if(nw_m > bg_m)
					rec = 1;
			}
			else if(nw_h == ed_h)//与终止时相等
			{
				if(nw_m == ed_m)
				{
					if(nw_s < ed_s)
						rec = 1;
					else
						rec = 0;
				}				
				else if(nw_m < ed_m)
					rec = 1;
				else if(nw_m > ed_m)
					rec = 0;
			}
			else
				rec = 1;
		}
	}
	return( rec );
}
//***********************************************
//判断当前日期与设定日期的关系
//-----------------------------------------------
//bg_y、bg_m、bg_d    设定时间段开始年时、月分、日秒
//ed_y、ed_m、ed_d    设定时间段结束年时、月分、日秒
//nw_y、nw_m、nw_d    当前年时、月分、日秒
//retuen  0-不包含 1-包含中  2-等于tt  3-等于dd
//***********************************************
u8 pd_day_baohan(u16 bg_y,u8 bg_m,u8 bg_d, u16 ed_y,u8 ed_m, u8 ed_d, u16 nw_y,u8 nw_m, u8 nw_d)
{
	unsigned char rec=0;
	
	if((nw_y == bg_y) && (nw_m == bg_m) && (nw_d == bg_d)) rec = 2;
	else if((nw_y == ed_y) && (nw_m == ed_m) && (nw_d == ed_d)) rec = 3;
	else
	{
		if((nw_y < bg_y) || (nw_y > ed_y)) rec = 0;//不在年/时范围中,不包含
		else//在年/时范围中
		{
			if((nw_m < bg_m) || (nw_m > ed_m)) rec = 0;//不在月/分范围内
			else//在月/分范围内
			{
				if(bg_m == ed_m)//同一个月/分内
				{
					if((nw_d < bg_d) || (nw_d > ed_d)) rec = 0;//不在日/秒范围内
					else rec = 1;
				}
				else//不在同一个月/分
				{
					if(nw_m == bg_m)
					{
						if(nw_d >= bg_d) rec =1;
						else rec =0;
					}
					else if(nw_m == ed_m)
					{
						if(nw_d <= ed_d) rec = 1;
						else rec = 0;
					}
					else if((nw_m > bg_m) && (nw_m < ed_m)) rec = 1;
				}
			}
		}
	}
	
	return ( rec );	
}
//***************************
//判断当前时间是否大于设定时间  
//时分秒
//des设定时间   now当前时间
//re 0-小于 1-等于  2-大于
//***************************
u8  pd_time(u16  des_h,u8  des_m,u8  des_s,u8  now_h,u8  now_m,u8  now_s)
{
	unsigned char  rec=0;

	if((now_h == des_h) && (now_m == des_m) && (now_s == des_s)) rec = 1;        //正好等于设定时间，定时到
	else if(now_h < des_h) rec = 0;                                      //时小于设定，定时未到
    else if(now_h == des_h){                                              //时等于设定时间
		      	if(now_m < des_m) rec = 0;                               //时等，分小于设定，定时未到		
				else if(now_m == des_m){                                  //时等，分等
					 	if(now_s < des_s) rec = 0;                       //时等，分等，秒小于设定，定时未到
						else if(now_s >= des_s) rec = 2;                 //时等，分等，秒等或大，定时到
				}
				else if(now_m > des_m) rec = 2;                          //时等，分大于定时，定时过
	}	
	else if(now_h > des_h) rec = 2;                                      //时大于设定，定时时间过
	return ( rec );	
}

//***************************
//判断当前时间是否大于设定时间  
//年月日
//des设定时间   now当前时间
//re 0-小于 1-等于  2-大于
//***************************
u8 Detect_Time_YMD(u16  des_y,u8  des_m,u8  des_d,u16 now_y,u8  now_m,u8  now_d)
{
	u32 des_ymd = 0, now_ymd = 0;

	des_ymd = des_y * 10000 + des_m * 100 + des_d;		//目标时间年月日
	now_ymd = now_y * 10000 + now_m * 100 + now_d;		//当前时间年月日
	
	if(now_ymd < des_ymd)
		return 0;
	else if(now_ymd == des_ymd)
		return 1;
	else
		return 2;
}

//*************************************************
//判断当前时间(年月日时分秒)与设定时间(年月日时分秒)的关系
//bg_y、bg_mon、bg_d、bg_h、bg_min、bg_s    设定时间段开始年、月、日、时、分、秒
//ed_y、ed_mon、ed_d、ed_h、ed_min、ed_s    设定时间段结束年、月、日、时、分、秒
//nw_y、nw_mon、nw_d、nw_h、nw_min、nw_s    当前年、月、日、时、分、秒
//retuen  0-不包含 1-包含中  2-等于tt  3-等于dd
//***********************************************
u8 pd_ymdhms_baohan(u16 bg_y,u8 bg_mon, u8 bg_d, u8 bg_h, u8 bg_min, u8 bg_s,
								u16 ed_y,u8 ed_mon, u8 ed_d, u8 ed_h, u8 ed_min, u8 ed_s,
								u16 nw_y,u8 nw_mon, u8 nw_d, u8 nw_h, u8 nw_min, u8 nw_s)
{
	u32 bg_ymd = 0,bg_hms = 0,ed_ymd = 0,ed_hms = 0,nw_ymd = 0,nw_hms = 0;

	bg_ymd = bg_y * 10000 + bg_mon * 100 + bg_d;		//起始时间年月日
	bg_hms = bg_h * 10000 + bg_min * 100 + bg_s;		//起始时间时分秒

	ed_ymd = ed_y * 10000 + ed_mon * 100 + ed_d;		//终止时间年月日
	ed_hms = ed_h * 10000 + ed_min * 100 + ed_s;		//终止时间时分秒
	
	nw_ymd = nw_y * 10000 + nw_mon * 100 + nw_d;	//当前时间年月日
	nw_hms = nw_h * 10000 + nw_min * 100 + nw_s;		//当前时间时分秒

	if(bg_ymd > ed_ymd)		//起始年月日大于终止年月日
		return 0;
	
	if(bg_ymd == ed_ymd)		//起始和终止的年月日相同
	{
		if(nw_ymd == bg_ymd)		//当前年月日和起始年月日相同,比较时分秒
		{
			if(nw_hms == bg_hms)			//起始时分秒相同	
				return 2;
			else if(nw_hms == ed_hms)		//终止时分秒相同
				return 3;
			else if((nw_hms > bg_hms) && (nw_hms < ed_hms))
				return 1;
			else 
				return 0;
		}
		else		//当前年月日与起始不等，则肯定不包含
			return 0;
	}
	else
	{
		if((nw_ymd > bg_ymd) && (nw_ymd < ed_ymd))		//年月日包含在起始终止之间
			return 1;
		else if(nw_ymd == bg_ymd)				//与起始年月日相同，比较时分秒
		{
			if(nw_hms == bg_hms)			//时分秒相同
				return 2;
			else if(nw_hms < bg_hms)
				return 0;
			else if(nw_hms > bg_hms)
				return 1;
		}
		else if(nw_ymd == ed_ymd)				//与终止年月日相同，比较时分秒
		{
			if(nw_hms == ed_hms)			//时分秒相同
				return 3;
			else if(nw_hms < ed_hms)
				return 1;
			else if(nw_hms > ed_hms)
				return 0;
		}
		else 
			return 0;
	}
	return 0;
}

//多项式为x^7+x^6+x^5+x^2+1
u8 CS_Check(u8 *data, u16 length)		//水资源通信协议CS校验
{
    u8 i;
    u8 crc_data = 0;        // Initial value
    while(length--)
    {
        crc_data ^= *data++;        // crc ^= *data; data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc_data & 0x80 )
                crc_data = (crc_data << 1) ^ 0xE5;        
            else
                crc_data <<= 1;
        }
    }
    return crc_data;
}


// 将十六进制转换为float形式  
float Hex_To_Float(unsigned char *Byte,int num)
{

	int i;
	float pfValue;
	char cByte[4]; 	  
	for (i=0;i<num;i++)  
	{  
		cByte[i] = Byte[num-1-i];   
	}	   
  	pfValue=*(float*)&cByte; 
	return  pfValue;
}



//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
