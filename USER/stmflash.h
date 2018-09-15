#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 #define UPDATA_EXIT 1

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 256 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 			0x08000000 									//STM32 FLASH����ʼ��ַ

#define ADDR_IAP_BASE            		0x08000000  		//IAP��ڵ�ַ40K		
#define ADDR_APP_OFFSET			0xA000
#define ADDR_APP_RUN    			ADDR_IAP_BASE + ADDR_APP_OFFSET  		//APP�������е�ַ 80K

//#define ADDR_PRO_DOWNLOAD    		ADDR_APP_RUN + 0x14000  		//���ص�ַ
//#define ADDR_UPDATA_SUCCESS_FLAG			0x0803F800		//���������־
#define ADDR_PRO_DOWNLOAD    		0x1FE0100  		//���ص�ַ
#define ADDR_UPDATA_SUCCESS_FLAG			0x1FE0000		//���������־

#define ADDR_REC_TOTAL_LEN 		0x0803F802		//���յ�������ܳ���
#define ADDR_REC_TOTAL_NUM 		0x0803F804		//���յ�������ܴ���
//#define ADDR_RECED_NUM			0x0803F806		//�ѽ�������


#define FLASH_Page_Bytes_num   		2048  //each page size 2K


u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

















