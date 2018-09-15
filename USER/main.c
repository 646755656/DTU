//****************************************Copyright (c)**************************************************
//��Ŀ����:   ����ˮ����
//--------------�ļ���Ϣ--------------------------------------------------------------------------------
//  ��   ��   ��      : main.c
//  ��   ��   ��      : ֣��
//  �޸�����          : 20180810
//  ��   ��           :  
//--------------Ӳ����Ϣ----------------------------------------------------------------------------
//  оƬ�ͺ�          : STM32F103C8T6
//  ����Ƶ��          : 8.000000 MHz
//  ����Ƶ��          : 72.00000 MHz
//--------------�˿���Ϣ--------------------------------------------------------------------------------
//COM1-4Gģ��SIM7600
//COM2-����
//COM3-��������
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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//�����ж����ȼ�����Ϊ��2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	delay_init();					//��ʱ��ʼ��
 	Timer_Init();				//��ʱ����ʼ��
	Gpio_Init(); 
	DTU_Reset();				//DTU��λ
 	UART_Init();					//���ڳ�ʼ��
	
	AT24CXX_Init();
	Read_Save_Data();			//��ȡ�洢����
	Global_Param_Init();			//ȫ�ֱ�����ʼ��
	Print_Ip_And_Port();			//��ӡIP�˿ں�
	
	Power_On_Init();				//������ʼ��
	
	Sim7600ce_GPS_Init_Block();
	
	WDG_Init();					//���Ź���ʼ��

	while(1)
 	{
		Rec_Uart_Data();		//���մ�������
		Send_Data_2_4G_Module();	//��4Gģ�鷢������
		Rec_4G_Module_Data();
	}
 }


