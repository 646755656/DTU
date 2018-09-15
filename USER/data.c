#include "data.h"
#include "sys.h"
#include "delay.h"
#include "watchdog.h"
#include "gpio.h"
#include "work.h"
#include "stmflash.h"
#include "rtc.h"
#include "at24c02.h"

struct ComAddr_define comAddr;

void Read_Save_Data(void)
{
	u8 FirstReadFlag = 0, FirstWriteFlag = 0x55;

	WatchDog();
	AT24CXX_Read(ADDR_FIRST_READ_FLAG, (u8 *)&FirstReadFlag, 1);
	if(FirstReadFlag != FirstWriteFlag)		//第一次，参数初始化成0
	{
		Reset_Com_Addr();
		
		AT24CXX_Write(ADDR_FIRST_READ_FLAG, (u8 *)&FirstWriteFlag, 1);
	}
	else
	{
		Read_Com_Addr();					//读设备地址
	}
}

void Read_Com_Addr(void)
{
	u8 i=0;
	
	WatchDog();
	
	AT24CXX_Read(ADDR_COM_ADDR, (u8 *)&comAddr.flag, sizeof(comAddr));

	for(i = 0; i < 4; i++)
	{
		if(comAddr.IP[0][i] == 0xFF)
			comAddr.IP[0][i] = 0;
		if(comAddr.IP[1][i] == 0xFF)
			comAddr.IP[1][i] = 0;
	}
	
	for(i = 0; i < 2; i++)
	{
		if(comAddr.port[i] == 0xFFFF)
			comAddr.port[i] = 0;
	}
}

void Write_Com_Addr(void)
{
	WatchDog();
	AT24CXX_Write(ADDR_COM_ADDR, (u8 *)&comAddr.flag, sizeof(comAddr));
}

void Reset_Com_Addr(void)
{
	clearbuff((u8 *)&comAddr.flag, sizeof(comAddr));
	Write_Com_Addr();
}



