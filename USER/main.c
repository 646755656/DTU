//****************************************Copyright (c)**************************************************
//项目名称:   电折水程序
//--------------文件信息--------------------------------------------------------------------------------
//  文   件   名      : main.c
//  创   建   人      : 郑哲
//  修改日期          : 20180810
//  描   述           :  
//--------------硬件信息----------------------------------------------------------------------------
//  芯片型号          : STM32F103C8T6
//  晶振频率          : 8.000000 MHz
//  工作频率          : 72.00000 MHz
//--------------端口信息--------------------------------------------------------------------------------
//COM1-4G模块SIM7600
//COM2-外设
//COM3-串口配置
//------------------------------------------------------------------------------------------------------

//********************************************************************************************************/
#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "timer.h"
#include "watchdog.h"
#include "gpio.h"
#include "data.h"
#include "work.h"
#include "stmflash.h"
#include "at24c02.h"

#define APPLICATION_VERSION     "20180810V1.0"


 int main(void)
 {
//	SCB->VTOR = FLASH_BASE | ADDR_APP_OFFSET; /* Vector Table Relocation in Internal FLASH. */
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置中断优先级分组为组2:2位抢占优先级，2位响应优先级
 	delay_init();					//延时初始化
 	Timer_Init();				//定时器初始化
	Gpio_Init(); 
	DTU_Reset();				//DTU复位
 	UART_Init();					//串口初始化
	
	AT24CXX_Init();
	Read_Save_Data();			//读取存储数据
	Global_Param_Init();			//全局变量初始化
	Print_Ip_And_Port();			//打印IP端口号
	
	Power_On_Init();				//开机初始化
	
	Sim7600ce_GPS_Init_Block();
	
	WDG_Init();					//看门狗初始化

	while(1)
 	{
		Rec_Uart_Data();		//接收串口数据
		Send_Data_2_4G_Module();	//向4G模块发送数据
		Rec_4G_Module_Data();
	}
 }


