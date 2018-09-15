#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 #define UPDATA_EXIT 1

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 256 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 			0x08000000 									//STM32 FLASH的起始地址

#define ADDR_IAP_BASE            		0x08000000  		//IAP入口地址40K		
#define ADDR_APP_OFFSET			0xA000
#define ADDR_APP_RUN    			ADDR_IAP_BASE + ADDR_APP_OFFSET  		//APP程序运行地址 80K

//#define ADDR_PRO_DOWNLOAD    		ADDR_APP_RUN + 0x14000  		//下载地址
//#define ADDR_UPDATA_SUCCESS_FLAG			0x0803F800		//升级程序标志
#define ADDR_PRO_DOWNLOAD    		0x1FE0100  		//下载地址
#define ADDR_UPDATA_SUCCESS_FLAG			0x1FE0000		//升级程序标志

#define ADDR_REC_TOTAL_LEN 		0x0803F802		//接收到程序的总长度
#define ADDR_REC_TOTAL_NUM 		0x0803F804		//接收到程序的总次数
//#define ADDR_RECED_NUM			0x0803F806		//已接收条数


#define FLASH_Page_Bytes_num   		2048  //each page size 2K


u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

















