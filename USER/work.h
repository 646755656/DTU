#ifndef __WORK_H
#define __WORK_H
#include "sys.h"
#include "data.h"


/*-------------------------------------------------------------------------------------------------------
  $+CGPSINFO:<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh
  <1>  纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
  <2>  纬度半球N（北半球）或S（南半球）
  <3>  经度dddmm.mmmm（度分）格式（前面的0也将被传输）
  <4>  经度半球E（东经）或W（西经）
  <5>  UTC日期，ddmmyy（日月年）格式
-------------------------------------------------------------------------------------------------------*/
struct GpsDataDefine
{
	int UTCDateTime[6];  	// YMDHMS
	char status;          	// A/V [ASCII]
	char NS;              	// N/S [ASCII] 
	char EW;              	// E/W [ASCII]
	float latitude;     	  // ddmm.mmmm [ASCII]  //取小数点3bit
	float longitude;      	// dddmm.mmmm [ASCII] //取小数点3bit
//	float speed;          	// 速率000.0~999.9节  x10//小数点1bit
//	float	course;         	// 航向000.0~359.9度  x10//小数点1bit
};




extern u32 sendNetConTimeCs;
extern u32 testTimeCs;

void Send_Data_2_4G_Module(void);

void DTU_Reset(void);
void  Power_On_Init(void);
void Print_Ip_And_Port(void);
void Global_Param_Init(void);

void Rec_Uart_Data(void);
u8 Rec_Terminal_Ip_And_Port(void);
u8 Sim7600ce_Net_Init(void);
u8 Sim7600ce_Pdp_Context_Enale(void);
u8 Sim7600ce_GPRS_Send_Data(u8 channel, u8 *sendBuf, u16 sendLen);
u8 Sim7600ce_Tcp_Client(u8 channel,u8 *ipAddr,u16 portNum);
u8 Sim7600ce_GPS_Init_Unblock(void);
void Sim7600ce_GPS_Init_Block(void);
u8 Analysis_GPS_Data(void);
u8 Extract_Comma_Between_Data(u8 *data, u8 *str, u8 begin, u8 end);
u8 Sim7600ce_Get_GPS(void);
u8 Sim7600ce_Send_Cmd(u8 *cmd,u8 *ack,u16 waittime);
void Send_4G_Mode_Get_GPS(void);
void Transmit_Upper_Data(void);
void Get_GPS_Data(void);
void Get_AGPS_Data(void);

void Error_Flag_Handle(void);
void Rec_4G_Module_Data(void);
u8 Find_c(u8* date, u8* str);



#endif
