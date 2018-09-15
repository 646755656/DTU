#include "work.h"
#include "watchdog.h"
#include "data.h"
#include "delay.h"
#include "uart.h"
#include "timer.h"
#include "stm32f10x_it.h"
#include "sys.h"
#include "gpio.h"
#include "stmflash.h"
#include "global.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

u8 failData[7]={0xAD,0x00,0x01,0x01,0x00,0x00,0x0};
u8 successData[7]={0xAD,0x00,0x01,0x01,0x01,0x00,0x0};

u8 netConnctIndex = 0;
u8 netInitIndex = 0;
u8 initFuncSelect = 0;

u8 pdpContextIndex = 0;
u8 pdpFuncSelect = 0;

u8 tcpClientIndex = 0;

u32 sendNetConTimeCs = 0;
u8 sendNetConErrCount = 0;

u8 connectFailCount = 0;		//连接失败次数

u8 sendUartBuf[1500];
u16 sendUartLen = 0;
u8 sendDataIndex = 0;

u8 rec4GDataBuf[1500];

u8 sendGPSInit = 0;
u8 getGPSDataMode = 0;

struct GpsDataDefine gpsData;
u8 getGPSSuccessFlag = 0;
u8 getGPSFailCount = 0;

u32 testTimeCs = 0;

void Send_Data_2_4G_Module(void)
{
	u8 revd = 0,flag = 0;
	char gpsBuf[30];
	WatchDog();
	
	switch(netConnctIndex)
	{
		case GET_4G_MODULE_GPS:
			revd = Sim7600ce_Get_GPS();
			if((revd == SUCCESSED_FLAG) || (revd == ERROR_FLAG))
			{
				getGPSFailCount = 0;
				netConnctIndex = NET_INIT;
			}
			break;
			
		case NET_INIT:
			revd = Sim7600ce_Net_Init();
			if(revd == SUCCESSED_FLAG)
			{
				connectFailCount = 0;
				netInitIndex = 0;
				sendNetConErrCount = 0;
				netConnctIndex = CONNECT_SERVER;
			}
			else if(revd == ERROR_FLAG)
			{
				connectFailCount++;
				Error_Flag_Handle();
			}
			break;
		case CONNECT_SERVER:
			revd = Sim7600ce_Pdp_Context_Enale();
			if(revd == SUCCESSED_FLAG)
			{
				connectFailCount = 0;
				pdpContextIndex = 0;
				sendNetConErrCount = 0;
				if((comAddr.IP[0][0] != 0) && (comAddr.IP[0][1] != 0) && (comAddr.port[0] != 0))
					netConnctIndex = TCP1_CONNECT;
				else if((comAddr.IP[1][0] != 0) && (comAddr.IP[1][1] != 0) && (comAddr.port[1] != 0))
					netConnctIndex = TCP2_CONNECT;
				else
					netConnctIndex = ERROR_RUN_STATUS;
			}
			else if(revd == ERROR_FLAG)
			{
				connectFailCount++;
				Error_Flag_Handle();
				
			}
			break;
		case TCP1_CONNECT:
			revd = Sim7600ce_Tcp_Client(0, (u8 *)&comAddr.IP[0][0], comAddr.port[0]);
			if(revd == SUCCESSED_FLAG)
			{
				connectFailCount = 0;
				UART2_PutString(successData, sizeof(successData));
				UART1_Clear();
				if((comAddr.IP[1][0] != 0) && (comAddr.IP[1][1] != 0) && (comAddr.port[1] != 0))
				{
					netConnctIndex = TCP2_CONNECT;
				}
				else
				{
					netConnctIndex = REC_TERMINAL_DATA;
					sendDataIndex = 0;
					UART2_Clear();
				}
			}
			else if(revd == ERROR_FLAG)
			{
				connectFailCount++;
				Error_Flag_Handle();
			}
			break;
			
		case TCP2_CONNECT:
			revd = Sim7600ce_Tcp_Client(1, (u8 *)&comAddr.IP[1][0], comAddr.port[1]);
			if(revd == SUCCESSED_FLAG)
			{
				connectFailCount = 0;
				sendDataIndex = 0;
				netConnctIndex = REC_TERMINAL_DATA;
				UART2_PutString(successData, sizeof(successData));
				UART1_Clear();
				UART2_Clear();
			}
			else if(revd == ERROR_FLAG)
			{
				connectFailCount++;
				Error_Flag_Handle();
			}
			break;

		case REC_TERMINAL_DATA:
			if((com2.jg_time >= TIME20MS) && (com2.rcv_len > 0))	
			{
				netConnctIndex = SEND_REC_DATA;
				sendUartLen = com2.rcv_len;
				clearbuff(sendUartBuf,sizeof(sendUartBuf));
				memcpy(sendUartBuf, com2.rcv_buff, sendUartLen);
			}
#if 0
			else
			{
				if(getGPSSuccessFlag == 0)
				{
					if(testTimeCs < TIME1S * 5)
						break;
		                    testTimeCs = 0;
					if(com1.rcv_len == 0)
					{
						Get_GPS_Data();
					}				
				}
			}
#endif
			break;
			
		case SEND_REC_DATA:
			revd = Sim7600ce_GPRS_Send_Data(0,sendUartBuf,sendUartLen);
			if(revd == SUCCESSED_FLAG)
			{
				connectFailCount = 0;
				netConnctIndex = REC_TERMINAL_DATA;
				UART2_Clear();
			}
			else if(revd == ERROR_FLAG)
			{
				connectFailCount++;
				Error_Flag_Handle();
			}
				
			break;
		case ERROR_RUN_STATUS:
			//检测IP和端口
			if((comAddr.IP[0][0] != 0) && (comAddr.IP[0][1] != 0) && (comAddr.port[0] != 0))
				netConnctIndex = TCP1_CONNECT;
			else if((comAddr.IP[1][0] != 0) && (comAddr.IP[1][1] != 0) && (comAddr.port[1] != 0))
				netConnctIndex = TCP2_CONNECT;
			break;
		default:
			netConnctIndex = NET_INIT;
			break;
	}
}

u8 Sim7600ce_Get_GPS(void)
{
	u8 atBuf[40] = {0};
	char *revd = NULL,flag = 0,gpsBuf[30] = {0};
	WatchDog();
	switch(getGPSDataMode)
	{
		case SEND_GPS_DATA:
			sprintf((char *)atBuf,"AT+CGPSINFO\r\n");
			
			UART1_Clear();
			UART1_SendStr(atBuf);
			getGPSDataMode = REC_GPS_DATA;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"0.0获取GPS数据:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;
			
		case REC_GPS_DATA:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					getGPSDataMode = SEND_GPS_DATA;
					flag = Analysis_GPS_Data();
					if(flag == SUCCESSED_FLAG)
					{
						sendNetConTimeCs = 0;
						getGPSSuccessFlag = 1;
						UART3_SendStr((u8 *)"0.0获取GPS数据成功!!!!\r\n");
						sprintf(gpsBuf,"经度:%f\r\n",gpsData.longitude);
						UART3_SendStr((u8 *)gpsBuf);	//打印
						sprintf(gpsBuf,"纬度:%f\r\n",gpsData.latitude);
						UART3_SendStr((u8 *)gpsBuf);	//打印
						UART3_SendStr((u8 *)"\r\n");
						return SUCCESSED_FLAG;
					}
					else
					{
						getGPSFailCount++;
						getGPSDataMode = GPS_IDLE_MODE;
						sendNetConTimeCs = 0;
						if(getGPSFailCount >= 3)
						{
							getGPSFailCount = 0;
							getGPSDataMode = SEND_GPS_DATA;
							UART3_SendStr((u8 *)"0.0获取GPS数据失败.....\r\n");
							return ERROR_FLAG;
						}
					}
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				getGPSFailCount++;
				getGPSDataMode = SEND_GPS_DATA;
				if(getGPSFailCount >= 3)
				{
					getGPSFailCount = 0;
					UART3_SendStr((u8 *)"0.0获取GPS数据失败.....\r\n");
					return ERROR_FLAG;
				}
			}
			break;
		case GPS_IDLE_MODE:
			if(sendNetConTimeCs >= TIME400MS)
			{
				getGPSDataMode = SEND_GPS_DATA;
			}
			break;
	}
	return NORMAL_FLAG;
}


u8 Sim7600ce_Net_Init(void)
{
	u8 atBuf[40] = {0};
	char *revd = NULL;
	WatchDog();
	switch(netInitIndex)
	{
		case SEND_INIT_IDLE_MODE:
			switch(initFuncSelect)
			{
				case 0: netInitIndex = SEND_INIT_TEST_UART; 		break;
				case 1: netInitIndex = SEND_INIT_ENTER_PIN; 		break;
				case 2: netInitIndex = SEND_INIT_SIGNAL_QUALITY; 	break;
				case 3: netInitIndex = SEND_INIT_REG_STATUS; 		break;
				case 4: netInitIndex = SEND_INIT_GPRS_STATUS; 		break;
			}
			break;
		case SEND_INIT_TEST_UART:			//确认串口正常
			sprintf((char *)atBuf, "AT\r\n");
			UART1_SendStr(atBuf);
			
			UART1_Clear();
			netInitIndex = REC_INIT_TEST_UART;
			sendNetConTimeCs = 0;
			UART3_SendStr((u8 *)"1.1确认串口是否正常:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_INIT_TEST_UART:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"1.1确认串口正常!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					netInitIndex = INIT_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				netInitIndex = SEND_INIT_TEST_UART;
				if(sendNetConErrCount >= 3)
				{
					UART3_SendStr((u8 *)"1.1初始化失败，模块重启............\r\n");
					sendNetConErrCount = 0;
					netInitIndex = SEND_INIT_IDLE_MODE;
					initFuncSelect = 0;
					return ERROR_FLAG;
				}
			}
			break;

		case SEND_INIT_ENTER_PIN:
			sprintf((char *)atBuf, "AT+CPIN?\r\n");
			UART1_SendStr(atBuf);
			UART1_Clear();
			netInitIndex = REC_INIT_ENTER_PIN;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"1.2检查SIM卡状态:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_INIT_ENTER_PIN:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"READY");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"1.2检查SIM卡状态成功!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
				
					netInitIndex = INIT_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				netInitIndex = SEND_INIT_ENTER_PIN;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					netInitIndex = SEND_INIT_IDLE_MODE;
					initFuncSelect = 0;
					UART3_SendStr((u8 *)"1.2检查SIM卡状态失败......\r\n");
					return ERROR_FLAG;
				}
			}
			break;

		case SEND_INIT_SIGNAL_QUALITY:
			sprintf((char *)atBuf, "AT+CSQ\r\n");
			UART1_SendStr(atBuf);
			UART1_Clear();
			netInitIndex = REC_INIT_SIGNAL_QUALITY;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"1.3查询信号强度:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_INIT_SIGNAL_QUALITY:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"1.3查询信号强度!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					netInitIndex = INIT_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				netInitIndex = SEND_INIT_SIGNAL_QUALITY;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					netInitIndex = SEND_INIT_IDLE_MODE;
					initFuncSelect = 0;
					UART3_SendStr((u8 *)"1.3查询信号强度失败......\r\n");
					return ERROR_FLAG;
				}
			}
			break;

		case SEND_INIT_REG_STATUS:
			sprintf((char *)atBuf, "AT+CREG?\r\n");
			UART1_SendStr(atBuf);
			UART1_Clear();
			netInitIndex = REC_INIT_REG_STATUS;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"1.4注册网络:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_INIT_REG_STATUS:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"0,1");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"1.4注册本地网络成功!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					netInitIndex = INIT_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
				else
				{
					revd = strstr((const char*)com1.rcv_buff,"0,5");
					if(revd != NULL)
					{
						UART3_SendStr((u8 *)"1.4注册漫游网络成功!!!\r\n");
						UART3_SendStr(com1.rcv_buff);	//打印
						UART3_SendStr((u8 *)"\r\n");
						
						netInitIndex = INIT_FUNCTION_SELECT;
						sendNetConTimeCs = 0;
						sendNetConErrCount = 0;
					}
				}
			}
			if(sendNetConTimeCs >= TIME1S * 2)
			{
				sendNetConErrCount++;
				netInitIndex = SEND_INIT_REG_STATUS;
				if(sendNetConErrCount >= 3)
				{
					sendNetConErrCount = 0;
					netInitIndex = SEND_INIT_IDLE_MODE;
					initFuncSelect = 0;
					UART3_SendStr((u8 *)"1.4注册网络失败......\r\n");
					return ERROR_FLAG;
				}
			}
			break;

			
		case SEND_INIT_GPRS_STATUS:
			sprintf((char *)atBuf, "AT+CGREG?\r\n");
			UART1_SendStr(atBuf);
			UART1_Clear();
			netInitIndex = REC_INIT_GPRS_STATUS;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"1.5查询GPRS网络注册状态:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_INIT_GPRS_STATUS:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"1.5查询GPRS网络注册状态!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					netInitIndex = INIT_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				netInitIndex = SEND_INIT_GPRS_STATUS;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					netInitIndex = SEND_INIT_IDLE_MODE;
					initFuncSelect = 0;
					UART3_SendStr((u8 *)"1.5GPRS网络注册失败.....\r\n");
					return ERROR_FLAG;
				}
			}
			break;

		case INIT_FUNCTION_SELECT:
			if(sendNetConTimeCs >= TIME200MS)
			{
				netInitIndex = SEND_INIT_IDLE_MODE;
				sendNetConErrCount = 0;
				initFuncSelect++;
				if(initFuncSelect > 4)
				{
					initFuncSelect = 0;
					return SUCCESSED_FLAG;
				}
			}
			break;
			
		default:
			netInitIndex = SEND_INIT_TEST_UART;
			break;
	}
	return NORMAL_FLAG;
}

u8 Sim7600ce_Pdp_Context_Enale(void)
{
	u8 atBuf[40] = {0};
	char *revd = NULL;
	WatchDog();
	switch(pdpContextIndex)
	{
		case SEND_PDP_IDLE_MODE:
			switch(pdpFuncSelect)
			{
				case 0: pdpContextIndex = SEND_PDP_APN_SETTING; 			break;
				case 1: pdpContextIndex = SEND_PDP_PROFILE_NUMBER; 		break;
				case 2: pdpContextIndex = SEND_PDP_TCP_MODE; 			break;
				case 3: pdpContextIndex = SEND_PDP_NETOPEN; 				break;
				case 4: pdpContextIndex = SEND_PDP_IPADDR; 				break;
			}
			break;
		case SEND_PDP_APN_SETTING:			//APN设置
			sprintf((char *)atBuf, "AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
//			sprintf((char *)atBuf, "AT+CGSOCKCONT=1,\"IP\",\"cmnet\"\r\n");
			UART1_SendStr(atBuf);
			
			UART1_Clear();
			pdpContextIndex = REC_PDP_APN_SETTING;
			sendNetConTimeCs = 0;
			UART3_SendStr((u8 *)"2.1设置APN:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_PDP_APN_SETTING:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"2.1APN设置成功!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					pdpContextIndex = PDP_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				pdpContextIndex = SEND_PDP_APN_SETTING;
				if(sendNetConErrCount >= 2)
				{
					UART3_SendStr((u8 *)"2.1APN设置失败......\r\n");
					sendNetConErrCount = 0;
					pdpContextIndex = SEND_PDP_IDLE_MODE;
					pdpFuncSelect = 0;
					return ERROR_FLAG;
				}
			}
			break;

		case SEND_PDP_PROFILE_NUMBER:					//Set active PDP context's profile number
			sprintf((char *)atBuf, "AT+CSOCKSETPN=1\r\n");
			UART1_SendStr(atBuf);
			UART1_Clear();
			pdpContextIndex = REC_PDP_PROFILE_NUMBER;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"2.2设置PDP Number:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_PDP_PROFILE_NUMBER:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"2.2设置PDP Number成功!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
				
					pdpContextIndex = PDP_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				pdpContextIndex = SEND_PDP_PROFILE_NUMBER;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					pdpContextIndex = SEND_PDP_IDLE_MODE;
					pdpFuncSelect = 0;
					UART3_SendStr((u8 *)"2.2设置PDP Number失败.......\r\n");
					return ERROR_FLAG;
				}
			}
			break;

		case SEND_PDP_TCP_MODE:							//Select TCP/IP application mode
			sprintf((char *)atBuf, "AT+CIPMODE=0\r\n");		//Non transparent mode
			UART1_SendStr(atBuf);
			UART1_Clear();
			pdpContextIndex = REC_PDP_TCP_MODE;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"2.3设定非透传模式:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_PDP_TCP_MODE:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"2.3设定非透传模式成功!!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					pdpContextIndex = PDP_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				pdpContextIndex = SEND_INIT_SIGNAL_QUALITY;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					pdpContextIndex = SEND_PDP_IDLE_MODE;
					pdpFuncSelect = 0;
					UART3_SendStr((u8 *)"2.3设定非透传模式成功......\r\n");
					return ERROR_FLAG;
				}
			}
			break;

		case SEND_PDP_NETOPEN:					//Open socket
			sprintf((char *)atBuf, "AT+NETOPEN\r\n");
			UART1_SendStr(atBuf);
			UART1_Clear();
			pdpContextIndex = REC_PDP_NETOPEN;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"2.4打开socket:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_PDP_NETOPEN:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"2.4打开socket成功!!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					pdpContextIndex = PDP_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				pdpContextIndex = SEND_PDP_NETOPEN;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					pdpContextIndex = SEND_PDP_IDLE_MODE;
					pdpFuncSelect = 0;
					UART3_SendStr((u8 *)"2.4打开socket失败......\r\n");
					return ERROR_FLAG;
				}
			}
			break;

			
		case SEND_PDP_IPADDR:					//Inquire socket PDP address
			sprintf((char *)atBuf, "AT+IPADDR\r\n");
			UART1_SendStr(atBuf);
			UART1_Clear();
			pdpContextIndex = REC_PDP_IPADDR;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"2.5查询socket PDP地址:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;

		case REC_PDP_IPADDR:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"2.5查询socket PDP地址成功!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					pdpContextIndex = PDP_FUNCTION_SELECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				pdpContextIndex = SEND_PDP_IPADDR;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					pdpContextIndex = SEND_PDP_IDLE_MODE;
					pdpFuncSelect = 0;
					UART3_SendStr((u8 *)"2.5查询socket PDP地址失败......\r\n");
					return ERROR_FLAG;
				}
			}
			break;

		case PDP_FUNCTION_SELECT:
			if(sendNetConTimeCs >= TIME200MS)
			{
				pdpContextIndex = SEND_PDP_IDLE_MODE;
				pdpFuncSelect++;
				if(pdpFuncSelect > 4)
				{
					pdpFuncSelect = 0;
					return SUCCESSED_FLAG;
				}
			}
			break;
			
		default:
			pdpContextIndex = SEND_PDP_IDLE_MODE;
			pdpFuncSelect = 0;
			break;
	}
	return NORMAL_FLAG;
}

u8 Sim7600ce_Tcp_Client(u8 channel,u8 *ipAddr,u16 portNum)
{
	u8 atBuf[50],ipBuf[20] = {0},retBuf[15] = {0};
	char *revd = NULL;
	WatchDog();
	
	switch(tcpClientIndex)
	{
		case SEND_TCP_FIRST:
			sprintf((char *)atBuf,"AT+CIPOPEN?\r\n");
			
			UART1_SendStr(atBuf);
			UART1_Clear();
			tcpClientIndex = REC_TCP_FIRST;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"3.0查询TCP是否连接:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;
			
		case REC_TCP_FIRST:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				UART3_SendStr((u8 *)"3.0查询TCP连接信息!!!!\r\n");
				UART3_SendStr(com1.rcv_buff);	//打印
				UART3_SendStr((u8 *)"\r\n");
				
				tcpClientIndex = SEND_TCP_ESTABLISH_CONNECT;
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				tcpClientIndex = SEND_TCP_FIRST;
				if(sendNetConErrCount >= 2)
				{
					tcpClientIndex = SEND_TCP_FIRST;
					sendNetConErrCount = 0;
					UART3_SendStr((u8 *)"3.0建立TCP未连接.....\r\n");
					return ERROR_FLAG;
				}
			}
			break;
			
		case SEND_TCP_ESTABLISH_CONNECT:
			sprintf((char *)ipBuf, "%d.%d.%d.%d",ipAddr[0],ipAddr[1],ipAddr[2],ipAddr[3]);
			sprintf((char *)atBuf,"AT+CIPOPEN=%d,\"TCP\",\"%s\",%d\r\n",channel,ipBuf,portNum);

#if 1
			UART1_SendStr(atBuf);
			UART1_Clear();
			tcpClientIndex = REC_TCP_ESTABLISH_CONNECT;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"3.1建立TCP连接:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");

#else
			sprintf((char *)retBuf, "+CIPOPEN: %d,0",channel);
			UART3_SendStr((u8 *)"3.1建立TCP连接:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			flag = Sim7600ce_Send_Cmd(atBuf,retBuf,400);
			if(flag == 1)
			{
				tcpClientIndex = SEND_TCP_INQUIRY_CONNECT;
				sendNetConTimeCs = 0;
				sendNetConErrCount = 0;
				UART3_SendStr((u8 *)"3.1建立TCP连接成功!!!\r\n");
				UART3_SendStr(com1.rcv_buff);
				UART3_SendStr((u8 *)"\r\n");
			}
			else
			{
				sendNetConErrCount++;
				tcpClientIndex = SEND_TCP_ESTABLISH_CONNECT;
				if(sendNetConErrCount >= 2)
				{
					tcpClientIndex = SEND_TCP_FIRST;
					sendNetConErrCount = 0;
					UART3_SendStr((u8 *)"3.1建立TCP连接失败......\r\n");
					return ERROR_FLAG;
				}
			}
#endif
			break;

		case REC_TCP_ESTABLISH_CONNECT:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				sprintf((char *)retBuf, "+CIPOPEN: %d,0",channel);
				revd = strstr((const char*)com1.rcv_buff,(char *)retBuf);
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"3.1建立TCP连接成功!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					tcpClientIndex = SEND_TCP_INQUIRY_CONNECT;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
					return SUCCESSED_FLAG;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				tcpClientIndex = SEND_TCP_ESTABLISH_CONNECT;
				if(sendNetConErrCount >= 2)
				{
					tcpClientIndex = SEND_TCP_FIRST;
					sendNetConErrCount = 0;
					UART3_SendStr((u8 *)"3.1建立TCP连接失败......\r\n");
				}
			}
			break;

		case SEND_TCP_INQUIRY_CONNECT:
			sprintf((char *)atBuf,"AT+CIPOPEN?\r\n");
			
			UART1_SendStr(atBuf);
			UART1_Clear();
			tcpClientIndex = REC_TCP_INQUIRY_CONNECT;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"3.2查询TCP是否连接:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;
			
		case REC_TCP_INQUIRY_CONNECT:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				sprintf((char *)retBuf, "%d,\"TCP\",",channel);
				revd = strstr((const char*)com1.rcv_buff,(char *)retBuf);
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"3.2建立TCP已连接!!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					tcpClientIndex = SEND_TCP_FIRST;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				tcpClientIndex = SEND_TCP_INQUIRY_CONNECT;
				if(sendNetConErrCount >= 2)
				{
					tcpClientIndex = SEND_TCP_FIRST;
					sendNetConErrCount = 0;
					UART3_SendStr((u8 *)"3.2建立TCP未连接.....\r\n");
					return ERROR_FLAG;
				}
			}
			break;
			
		default:
			tcpClientIndex = 0;
			break;
	}
	return NORMAL_FLAG;
}

u8 Sim7600ce_GPRS_Send_Data(u8 channel, u8 *sendBuf, u16 sendLen)
{
	 u8 atBuf[30] = {0},printBuf[4] = {0};
	 u16 i;
	 char *revd = NULL;
	 WatchDog();
	 
	switch(sendDataIndex)
	{
		case SEND_DATA_SET_CHANNEL:
			 sprintf((char*)atBuf,"AT+CIPSEND=%d,%d\r\n",channel,sendLen);
#if 0
			flag = Sim7600ce_Send_Cmd(atBuf,(u8 *)">",300);
			if(flag == 1)
			{
				sendDataIndex = SEND_DATA_TCP_MODE;
				sendNetConTimeCs = 0;
				sendNetConErrCount = 0;
			}
			else
			{
				sendNetConErrCount++;
				sendDataIndex = SEND_DATA_SET_CHANNEL;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					return ERROR_FLAG;
				}
			}

#else
			UART1_SendStr(atBuf);
			UART1_Clear();
			sendDataIndex = REC_DATA_SET_CHANNEL;
			sendNetConTimeCs = 0;
			UART3_SendStr((u8 *)"4.1准备发送数据到服务器:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
#endif
			break;
			
		case REC_DATA_SET_CHANNEL:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,">");
				if(revd != NULL)
				{
					sendDataIndex = SEND_DATA_TCP_MODE;
					sendNetConTimeCs = 0;
					sendNetConErrCount = 0;
					
					UART3_SendStr((u8 *)"4.1开始发送数据....\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					UART1_Clear();
				}
			}
			if(sendNetConTimeCs >= TIME1S * 2)
			{
				sendDataIndex = SEND_DATA_SET_CHANNEL;
				sendNetConErrCount++;
				UART1_Clear();
				return ERROR_FLAG;
			}
			break;

		case SEND_DATA_TCP_MODE:
			UART3_SendStr((u8 *)"4.2正在发送数据....\r\n");
			UART1_PutString(sendUartBuf,sendUartLen);
			UART1_Clear();

			for(i = 0; i < sendUartLen; i++)
			{
				sprintf(printBuf,"%02x ",sendUartBuf[i]);
				UART3_SendStr(printBuf);
			}
			UART3_SendStr((u8 *)"\r\n");
			
			sendDataIndex = SEND_DATA_SET_CHANNEL;
			UART3_SendStr((u8 *)"4.2数据发送完毕....\r\n");
			return SUCCESSED_FLAG;

		default:
			sendDataIndex = SEND_DATA_SET_CHANNEL;
			break;
	}
	return NORMAL_FLAG;
}

void Rec_4G_Module_Data(void)
{
	u8 flag = 0;
	char *revd = NULL;
	char gpsBuf[30] = {0};
	WatchDog();

	if(netConnctIndex != REC_TERMINAL_DATA) 
		return;
//Get_AGPS_Data();	

	if(getGPSSuccessFlag == 0)		//获取GPS数据
	{
		if(testTimeCs >= TIME1S * 3)
		{
			testTimeCs = 0;
			if((com1.rcv_len == 0) && (com2.rcv_len == 0))		//串口1和2都没有收到数据
			{
//				Send_4G_Mode_Get_GPS();
			}
		}
	}

	if((com1.jg_time >= TIME20MS) && (com1.rcv_len >0))
	{
		revd = strstr((char *)com1.rcv_buff, "+IPD");			// 4G模块接收到上位机发来的数据
		if(revd != NULL)
		{
			memset(rec4GDataBuf,0,sizeof(rec4GDataBuf));
			memcpy(rec4GDataBuf,com1.rcv_buff,com1.rcv_len );//拷贝出4G模块发来的数据
			Transmit_Upper_Data();		//转发上位机发来的数据
			return;
		}
		
		revd = strstr((char *)com1.rcv_buff, "+CGPSINFO:");		//经纬度数据
		if(revd != NULL)
		{
			flag = Analysis_GPS_Data();
			if(flag == SUCCESSED_FLAG)
			{
				getGPSSuccessFlag = 1;
				UART3_SendStr((u8 *)"0.0获取GPS数据成功!!!!\r\n");
				sprintf(gpsBuf,"经度:%f\r\n",gpsData.longitude);
				UART3_SendStr((u8 *)gpsBuf);	//打印
				sprintf(gpsBuf,"纬度:%f\r\n",gpsData.latitude);
				UART3_SendStr((u8 *)gpsBuf);	//打印
				UART3_SendStr((u8 *)"\r\n");
			}
			else
			{
//				getGPSFailCount++;
				if(getGPSFailCount >= 10)
				{
					getGPSSuccessFlag = 1;
					getGPSFailCount = 0;
					clearbuff((u8 *)&gpsData.status, sizeof(gpsData));
					UART3_SendStr((u8 *)"0.0获取GPS数据失败.....\r\n");
				}
			}
			UART1_Clear();
			return;
		}
		else
			UART1_Clear();
	}
}

void Transmit_Upper_Data(void)
{
	char *revd = NULL;
	u8 buf[4] = {0},sendBuf[1500]={0},flag = 0,printBuf[4] = {0};
	u16 i,dataLen = 0;
	WatchDog();
	
	revd = strstr((const char *)rec4GDataBuf,"+IPD");
	if(revd != NULL)
	{
		buf[0] = *(revd + 4);
		buf[1] = *(revd + 5);
		dataLen = (buf[0] - 0x30) * 10 + (buf[1] - 0x30);
		
		if(dataLen > 1500) 
		{
			dataLen = 0;
			UART1_Clear();
		}
		
		memcpy(sendBuf,revd+8,dataLen);//拷贝出所有用户数据
		
		UART2_PutString(sendBuf, dataLen);
		
		UART3_SendStr((u8 *)"转发上位机下发的数据!!!!!!!!!!!!\r\n");
		for(i = 0; i < dataLen; i++)
		{
			sprintf(printBuf,"%02x ",sendBuf[i]);
			UART3_SendStr(printBuf);
		}
		UART3_SendStr((u8 *)"\r\n");
	}
	UART1_Clear();
}



u8 Sim7600ce_GPS_Init_Unblock(void)	//非阻塞式
{
	u8 atBuf[40] = {0};
	char *revd = NULL;
	WatchDog();
	switch(sendGPSInit)
	{
		case SEND_GPS_INIT_START:
			sprintf((char *)atBuf,"AT+CGPSAUTO=1\r\n");
			
			UART1_SendStr(atBuf);
			UART1_Clear();
			sendGPSInit = REC_GPS_INIT_START;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"0.0自动开启GPS:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;
			
		case REC_GPS_INIT_START:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"0.0自动开启GPS成功!!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					sendGPSInit = SEND_GPS_INIT_CLOSE_AGPS;
					sendNetConTimeCs = 0;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				sendGPSInit = SEND_GPS_INIT_START;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					UART3_SendStr((u8 *)"0.0自动开启GPS失败.....\r\n");
					return ERROR_FLAG;
				}
			}
			break;
			
		case SEND_GPS_INIT_CLOSE_AGPS:
			sprintf((char *)atBuf,"AT+CGPSSSL=0\r\n");
			
			UART1_SendStr(atBuf);
			UART1_Clear();
			sendGPSInit = REC_GPS_INIT_CLOSE_AGPS;
			sendNetConTimeCs = 0;
			
			UART3_SendStr((u8 *)"0.1关闭AGPS:\r\n");
			UART3_SendStr(atBuf);
			UART3_SendStr((u8 *)"\r\n");
			break;
			
		case REC_GPS_INIT_CLOSE_AGPS:
			if((com1.jg_time >= TIME20MS) && (com1.rcv_len > 0))
			{
				revd = strstr((const char*)com1.rcv_buff,"OK");
				if(revd != NULL)
				{
					UART3_SendStr((u8 *)"0.1关闭AGPS成功!!!!\r\n");
					UART3_SendStr(com1.rcv_buff);	//打印
					UART3_SendStr((u8 *)"\r\n");
					
					return SUCCESSED_FLAG;
				}
			}
			if(sendNetConTimeCs >= TIME500MS)
			{
				sendNetConErrCount++;
				sendGPSInit = SEND_GPS_INIT_CLOSE_AGPS;
				if(sendNetConErrCount >= 2)
				{
					sendNetConErrCount = 0;
					UART3_SendStr((u8 *)"0.1关闭AGPS失败.....\r\n");
					return ERROR_FLAG;
				}
			}
			break;
	}
	return NORMAL_FLAG;
}

void Sim7600ce_GPS_Init_Block(void)	//阻塞式
{
	u8 revd = 0;
	WatchDog();
	
	revd = Sim7600ce_Send_Cmd((u8 *)"AT+CGPSAUTO=1",(u8 *)"OK",100);
	if(revd == 0)
		Sim7600ce_Send_Cmd((u8 *)"AT+CGPSAUTO=1",(u8 *)"OK",100);
//	delay_ms(10);
//	revd = Sim7600ce_Send_Cmd((u8 *)"AT+CGPS=1",(u8 *)"OK",100);
//	if(revd == 0)
//		Sim7600ce_Send_Cmd((u8 *)"AT+CGPS=1",(u8 *)"OK",100);
	revd = Sim7600ce_Send_Cmd((u8 *)"AT+CGPSSSL=0",(u8 *)"OK",100);
	if(revd == 0)
		Sim7600ce_Send_Cmd((u8 *)"AT+CGPSSSL=0",(u8 *)"OK",100);
}

void Send_4G_Mode_Get_GPS(void)
{
	u8 atBuf[40] = {0};
	WatchDog();
	sprintf((char *)atBuf,"AT+CGPSINFO\r\n");
	
	UART1_Clear();
	UART1_SendStr(atBuf);
	
	UART3_SendStr((u8 *)"0.0获取GPS数据:\r\n");
	UART3_SendStr(atBuf);
	UART3_SendStr((u8 *)"\r\n");
}

u8 Analysis_GPS_Data(void)
{
	char *p1 = NULL,*p2 = NULL,*p3 = NULL;		//p1:起始位置,p2:数据起始位置,p3:移动指针，每次指向数据首位置
	u8 i,gpsBuf[200]= {0},buf[30] = {0},flag = 0;
	int numLocation = 0,data = 0;
	const char s1[2] = ": ";
	const char s2[2] = ",";
	
	WatchDog();
	
	UART3_SendStr((u8 *)"解析GPS数据\r\n");
	for(i = 0; i < 200; i++)
		gpsBuf[i] = com1.rcv_buff[i];
	
	p1 = strstr((const char *)gpsBuf,"+CGPSINFO:");	//开始位置
	p2 = strstr((const char *)gpsBuf,"\r\n\r\nOK");			//结束位置
	if((p1 == NULL) || (p2 == NULL))
		return ERROR_FLAG;
	
	*p2 = '\0';			//添加结束标志
	p2 = strtok(p1, s1);	//冒号分隔，冒号前面数据
	p2 = strtok(NULL, s1);	//冒号后面的数据，真正需要提取的数据，p2指向
	
	/* 解析--纬度 */	
	p3 = strtok(p2, s2);	//逗号分隔，p3指向数据
	if(p3 == NULL)
		return ERROR_FLAG;
	gpsData.latitude = strtod((const char*)p3,NULL);
	gpsData.latitude = ((gpsData.latitude /100 - (int)(gpsData.latitude /100)) * 100) / 60 + (int)(gpsData.latitude / 100);//转化为dd.ddddddd

	/* 解析--南北半球 */
	p3 = strtok(NULL, s2);	//逗号分隔，p3指向数据
	if(p3 == NULL)
		return ERROR_FLAG;
	gpsData.NS = *p3;

	/* 解析--经度 */
	p3 = strtok(NULL, s2);	//逗号分隔，p3指向数据
	if(p3 == NULL)
		return ERROR_FLAG;
	gpsData.longitude = strtod((const char*)p3,NULL); //经度格式为dddmm.mmmm 
	gpsData.longitude = ((gpsData.longitude / 100-(int)(gpsData.longitude / 100)) * 100) / 60 + (int)(gpsData.longitude / 100);//转化为ddd.ddddddd
	numLocation = gpsData.longitude / 15 + 1;//判断当前坐标在第几区，15度为一区，为后面算时间做准备


	/* 解析--东西半球 */	
	p3 = strtok(NULL, s2);	//逗号分隔，p3指向数据
	if(p3 == NULL)
		return ERROR_FLAG;
	gpsData.EW = *p3;				

	/* 解析--年月日 */		
	p3 = strtok(NULL, s2);	//逗号分隔，p3指向数据
	if(p3 == NULL)
		return ERROR_FLAG;
	data = atoi((const char*)p3);
	gpsData.UTCDateTime[0] = data % 100 ;
	gpsData.UTCDateTime[1] = data % 10000 / 100;
	gpsData.UTCDateTime[2] = data / 10000;

	/* 解析--时分秒 */
	p3 = strtok(NULL, s2);	//逗号分隔，p3指向数据
	if(p3 == NULL)
		return ERROR_FLAG;
	data = atoi((const char*)p3);//转换为float型

	if(gpsData.EW == 'E')//如果是东经
		gpsData.UTCDateTime[3] = data / 10000 + numLocation;//加上地区时
	else if(gpsData.EW == 'W')//如果是西经
		gpsData.UTCDateTime[3] = data / 10000 - numLocation;//减去地区时
		
	if(gpsData.UTCDateTime[3] >= 24)
	{
		gpsData.UTCDateTime[2]++; //日期超前一天
		gpsData.UTCDateTime[3] -= 24;
	}
	else if(gpsData.UTCDateTime[3] < 0)
	{
		gpsData.UTCDateTime[2]--;	 //日期落后一天
		gpsData.UTCDateTime[3] += 24;
	}
	gpsData.UTCDateTime[4] = data % 10000 / 100;
	gpsData.UTCDateTime[5] = data % 100;
	
	return SUCCESSED_FLAG;
}

void Get_GPS_Data(void)
{
	u8 flag = 0;
	char *revd = NULL,gpsBuf[30] = {0};
	WatchDog();
	
	flag = Sim7600ce_Send_Cmd((u8 *)"AT+CGPSINFO",(u8 *)"+CGPSINFO:",20);
	{
		flag = Analysis_GPS_Data();
		if(flag == SUCCESSED_FLAG)
		{
			getGPSSuccessFlag = 1;
			UART3_SendStr((u8 *)"0.0获取GPS数据成功!!!!\r\n");
			sprintf(gpsBuf,(char *)"经度:%f\r\n",gpsData.longitude);
			UART3_SendStr((u8 *)gpsBuf);	//打印
			sprintf(gpsBuf,(char *)"纬度:%f\r\n",gpsData.latitude);
			UART3_SendStr((u8 *)gpsBuf);	//打印
			UART3_SendStr((u8 *)"\r\n");
		}
		else
		{
//			getGPSFailCount++;
			if(getGPSFailCount >= 3)
			{
				getGPSSuccessFlag = 1;
				getGPSFailCount = 0;
				clearbuff((u8 *)&gpsData.status, sizeof(gpsData));
				UART3_SendStr((u8 *)"0.0获取GPS数据失败.....\r\n");
			}
		}
	}
}


void Get_AGPS_Data(void)
{
	u8 flag = 0;
	char *revd = NULL;
	WatchDog();
	flag = Sim7600ce_Send_Cmd((u8 *)"AT+CGSOCKCONT=1,\"IP\",\"cmnet\"\r\n",(u8 *)"OK",20);
	if(flag == 0)
		Sim7600ce_Send_Cmd((u8 *)"AT+CGSOCKCONT=1,\"IP\",\"cmnet\"\r\n",(u8 *)"OK",20);

	flag = Sim7600ce_Send_Cmd((u8 *)"AT+CGPSURL=\"114,215,29,22:5034\"\r\n",(u8 *)"OK",20);
	if(flag == 0)
		Sim7600ce_Send_Cmd((u8 *)"AT+CGPSURL=\"114,215,29,22:5034\"\r\n",(u8 *)"OK",20);
	
	flag = Sim7600ce_Send_Cmd((u8 *)"AT+CGPSSSL=0",(u8 *)"OK",20);
	if(flag == 0)
		Sim7600ce_Send_Cmd((u8 *)"AT+CGPSSSL=0",(u8 *)"OK",20);
	
	flag = Sim7600ce_Send_Cmd((u8 *)"AT+CGPS=1,3",(u8 *)"OK",20);
	if(flag == 0)
		Sim7600ce_Send_Cmd((u8 *)"AT+CGPS=1,3",(u8 *)"OK",20);

	while(1)
	{
		flag = Sim7600ce_Send_Cmd((u8 *)"AT+CGPSINFO",(u8 *)"OK",20);		//获取GPS
		if(flag == 1)
		{
			revd = strstr((char *)com1.rcv_buff, "+CGPSINFO:");		//经纬度数据
			if(revd != NULL)
			{
				flag = Analysis_GPS_Data();
				if(flag == SUCCESSED_FLAG)
				{
					getGPSSuccessFlag = 1;
				}
				else
				{
	//				getGPSFailCount++;
					if(getGPSFailCount >= 10)
					{
	//					getGPSSuccessFlag = 1;
	//					getGPSFailCount = 0;
						clearbuff((u8 *)&gpsData.status, sizeof(gpsData));
						UART3_SendStr((u8 *)"0.0获取GPS数据失败.....\r\n");
					}
				}
				UART1_Clear();
			}
		}
		delay_ms(1000);
		delay_ms(1000);
	}

}

//data:需要提取的字符串
//str:提取的数据
//begin:逗号起始位置
//end:逗号结束位置
u8 Extract_Comma_Between_Data(u8 *data, u8 *str, u8 begin, u8 end)	//提取逗号之间的数据
{
	int i=0,count=0;
	int beginPos = 0, endPos = 0;

	if(begin == end)
	{
		*str = '\0';
		return 0;
	}
	for(i = 0;i < 200; i++)		 //遍历数组
	{
		if(data[i] == ',')			 //寻找，逗号分割符号
		{
			if(count == begin)
				beginPos = i+1;		 //取出开始下标
			else if(count == end)
			{
				endPos = i;			 //取出结束下标
				break;
			}
			count++;				 //次数加
		}			
	}
	if(beginPos == endPos)
	{
		*str = '\0';
		return 0;
	}
	for(i = beginPos; i < endPos; i++)			 //取出数据赋值到str中
		*str++ = data[i];
	*str = '\0';					 //结尾补上‘\0’
	return 1;
}

u8 Find_c(u8* date, u8* str) //逗号分隔函数
{
	int i=4;

	for(i=4;i<210;i++)		 //遍历数组,跳过+IPD。后面是数据长度字符串
	{
		if(date[i]=='\r')			 //寻找\r\n，前面是数据区长度字符串
			break;	
		else
			*str++ = date[i];	//取出数据长度字符串		
	}	
	*str = '\0';					 //结尾补上‘\0’
	return (i+2);
} 

//接收串口数据
void Rec_Uart_Data(void)
{
	u8 revd = 0;
	WatchDog();

	if((com3.jg_time >= TIME20MS) && (com3.rcv_len > 0))
	{
		revd = Rec_Terminal_Ip_And_Port();//通过终端给DTU配置IP
		if(revd == 1)
			return;
	}
}

//接收终端发送的IP和端口号
u8 Rec_Terminal_Ip_And_Port(void)
{
	u8 i,j,pos = 4;
	u8 buf[7] = {0xDD,1,1,1,0,0,0};

	WatchDog();
	// DD 01 01 LEN IP1[0] IP1[1] IP1[2] IP1[3] PORT1[0] PORT1[1] IP2[0] IP2[1] IP2[2] IP2[3] PORT2[0] PORT2[1] CL CH
	
	if((com3.rcv_buff[0] == 0xDD) && (com3.rcv_buff[1] == 1) && (com3.rcv_buff[2] == 1))
	{
		if(com3.rcv_buff[3] == com3.rcv_len - 6)
		{
			CRC16(com3.rcv_buff, com3.rcv_len-2);
			if((com3.rcv_buff[com3.rcv_len-2] == uchCRCLo) && (com3.rcv_buff[com3.rcv_len-1] == uchCRCHi))
			{
				//IP
				for(i = 0; i < 2; i++)
				{
					for(j = 0; j < 4; j++)
						comAddr.IP[i][j] = com3.rcv_buff[pos++];
				}

				//PORT
				for(i = 0; i < 2; i++)
				{
					comAddr.port[i] = change_HEX_to_DEC((u8 *)&com3.rcv_buff[pos], 2);
					pos += 2;
				}
				Write_Com_Addr();		//保存
				
				buf[4] = 1;
				CRC16(buf, 5);
				buf[5] = uchCRCLo;
				buf[6] = uchCRCHi;
				UART3_PutString(buf,7);
			}
		}
		else
		{
			buf[4] = 0;
			CRC16(buf, 5);
			buf[5] = uchCRCLo;
			buf[6] = uchCRCHi;
			UART3_PutString(buf,7);
		}
		UART3_Clear();
		return 1;
	}
	else
	{
		buf[4] = 0;
		CRC16(buf, 5);
		buf[5] = uchCRCLo;
		buf[6] = uchCRCHi;
		UART3_PutString(buf,7);
	}
	return 0;
}


//开机初始化
void  Power_On_Init(void)
{
	u32  baund=0;
	u8 flag = 0;

	while(1)
	{
		if(flag == 0)
		{
			flag = 1;
			UART3_SendStr((u8 *)"模块初始化中......\r\n");
		}
		baund = 115200;
		USART1_Init(baund); 	// 4G模块设置串口
		UART1_Clear();		//清空串口1缓存和标志
		UART1_SendStr("AT\r\n");
		delay_ms(150);
		if(strstr((char*)com1.rcv_buff, "OK"))
			break;
		
		delay_ms(400);
//		baund = 921600;
//		USART1_Init(baund); 	// 4G模块设置串口
//		UART1_Clear();		//清空串口1缓存和标志
//		UART1_SendStr("AT\r\n");
//		delay_ms(150);
//		if(strstr((char*)com1.rcv_buff, "OK"))
//			break;
	}

//	if(baund == 115200)//如果4G模块波特率不是921600
//	{
//		UART1_Clear();		//清空串口1缓存和标志
//		UART1_SendStr("AT+IPR=921600\r\n");
//		USART1_Init(921600);
//		delay_ms(150);
//		UART3_SendStr(com1.rcv_buff);
//		delay_ms(100);		
//			
//	}

	UART1_Clear();	//清空串口1缓存和标志
	UART1_SendStr("AT+IPR?\r\n");//查询4G串口波特率
	delay_ms(150);
	UART3_SendStr(com1.rcv_buff);

}

/* Private define ------------------------------------------------------------*/
/**
  * @函数功能:  向sim7600ce发送命令
			           
  * @返回值  :  1,发送成功(得到了期待的应答结果)
                0,发送失败
  * @说  明  :  ①、cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
	        	 ②、ack:期待的应答结果,如果为空,则表示不需要等待应答
                ③、waittime:等待时间(单位:10ms)
*/
     
u8 Sim7600ce_Send_Cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res = 1; 
	char  atbuf[80] = {0};
	WatchDog(); //喂狗

	UART1_Clear();

	sprintf(atbuf,"%s\r\n" , cmd);

	UART1_SendStr((u8 *)atbuf);

	if(waittime>0)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			WatchDog(); //喂狗
			if(strstr((const char*)com1.rcv_buff,(const char*)ack))
			{
				waittime = 10;
				break;//得到有效数据 
			}
		}
		if(waittime==0)
		res = 0; 
	}
	return res;
} 

void Error_Flag_Handle(void)
{
	WatchDog();
	if(connectFailCount >= 10)
	{
		connectFailCount = 0;
		netConnctIndex = NET_INIT;
		netInitIndex = SEND_INIT_IDLE_MODE;
		initFuncSelect = 0;
		UART2_PutString(failData, sizeof(failData));

		UART3_SendStr((u8 *)"模块初始化失败,模块重启\r\n");

		DTU_Reset();		//复位DTU
		delay_ms(1000);
		WatchDog();
		delay_ms(500);
		WatchDog();
	}
}

void Global_Param_Init(void)
{
	CRC16(failData,5);
	failData[5] = uchCRCLo;
	failData[6] = uchCRCHi;

	CRC16(successData, 5);
	successData[5] = uchCRCLo;
	successData[6] = uchCRCHi;

	comAddr.IP[0][0] = 114;
	comAddr.IP[0][1] = 215;
	comAddr.IP[0][2] = 29;
	comAddr.IP[0][3] = 22;
	comAddr.port[0] = 5034;
	
//	comAddr.IP[0][0] = 39;
//	comAddr.IP[0][1] = 106;
//	comAddr.IP[0][2] = 122;
//	comAddr.IP[0][3] = 150;
//	comAddr.port[0] = 50349;
	
}

void Print_Ip_And_Port(void)
{
	u8 buf[30] = {0};
	WatchDog();
	
	sprintf((char *)buf,"IP1:%d.%d.%d.%d\r\n",comAddr.IP[0][0],comAddr.IP[0][1],comAddr.IP[0][2],comAddr.IP[0][3]);
	UART3_SendStr(buf);//打印IP地址1 
	
	sprintf((char *)buf,"PORT1:%d\r\n",comAddr.port[0]);
	UART3_SendStr(buf );//打印端口1

	sprintf((char *)buf,"IP2:%d.%d.%d.%d\r\n",comAddr.IP[1][0],comAddr.IP[1][1],comAddr.IP[1][2],comAddr.IP[1][3]);
	UART3_SendStr(buf);//打印IP地址1 
	
	sprintf((char *)buf,"PORT2:%d\r\n",comAddr.port[1]);
	UART3_SendStr(buf );//打印端口1
}

void DTU_Reset(void)
{
	DTU_RST_OUT = 0;
	delay_ms(500);
	DTU_RST_OUT = 1;
	delay_ms(110);
	DTU_RST_OUT = 0;
}




