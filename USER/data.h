#ifndef __DATA_H
#define __DATA_H

#include "stm32f10x.h"
#include "work.h"
#include "stmflash.h"
#include "global.h"


/************************************************/
#define ADDR_FIRST_READ_FLAG			0		
#define ADDR_COM_ADDR					1		


struct ComAddr_define
{
	u8 flag;
	u8 IP[2][4];		
	u16 port[2];		
};
extern struct ComAddr_define comAddr;


void Read_Save_Data(void);
void Read_Com_Addr(void);
void Write_Com_Addr(void);
void Reset_Com_Addr(void);


#endif



