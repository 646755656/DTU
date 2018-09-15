#ifndef __GLOBAL_H
#define __GLOBAL_H

typedef enum
{
	GET_4G_MODULE_GPS,	//获取GPS
	NET_INIT,				//网络初始化
	CONNECT_SERVER,		//连接服务器
	TCP1_CONNECT,			//连接TCP
	TCP2_CONNECT,			//连接TCP
	REC_TERMINAL_DATA,	//接收终端发送的数据
	SEND_REC_DATA,		//发送接收的数据
	ERROR_RUN_STATUS,	//没有IP和端口
	
}Net_Index;

typedef enum
{
	SEND_INIT_IDLE_MODE,
		
	SEND_INIT_TEST_UART,							//确认串口正常
	SEND_INIT_ENTER_PIN,					//查询SIM卡状态				
	SEND_INIT_SIGNAL_QUALITY,				//查询信号强度			
	SEND_INIT_REG_STATUS,					//注册到本地
	SEND_INIT_GPRS_STATUS,				//GPRS网络注册状态	
//	SEND_INIT_NETWORK_ATT,					//检查GPRS附着状态

	REC_INIT_TEST_UART,	
	REC_INIT_ENTER_PIN,							
	REC_INIT_SIGNAL_QUALITY,	
	REC_INIT_REG_STATUS,			
	REC_INIT_GPRS_STATUS,					//GPRS网络注册状态	
//	REC_INIT_NETWORK_ATT,					//检查GPRS附着状态
	
	INIT_FUNCTION_SELECT,
}Net_Init_Index;

typedef enum
{
	SEND_PDP_IDLE_MODE,
		
	SEND_PDP_APN_SETTING,					//APN设定
	SEND_PDP_PROFILE_NUMBER,
	SEND_PDP_TCP_MODE,			//Select TCP/IP application mode
	SEND_PDP_NETOPEN,
	SEND_PDP_IPADDR,					//查询当前激活socket PDP的IP地址
	

	REC_PDP_APN_SETTING,
	REC_PDP_PROFILE_NUMBER,
	REC_PDP_TCP_MODE,
	REC_PDP_NETOPEN,
	REC_PDP_IPADDR,
	
	PDP_FUNCTION_SELECT,
}PDP_Context_Index;

typedef enum
{
	SEND_TCP_FIRST,
	SEND_TCP_ESTABLISH_CONNECT,		//TCP连接
	SEND_TCP_INQUIRY_CONNECT,		//查询是否连接
	
	REC_TCP_FIRST,
	REC_TCP_ESTABLISH_CONNECT,
	REC_TCP_INQUIRY_CONNECT,
}TCP_Client_Index;

typedef enum
{
	SEND_DATA_SET_CHANNEL,		//设定发送数据通道
	SEND_DATA_TCP_MODE,
	
	REC_DATA_SET_CHANNEL,
	
}Send_Data_Index;

typedef enum
{
	SEND_GPS_INIT_START,
	SEND_GPS_INIT_CLOSE_AGPS,
	
	REC_GPS_INIT_START,
	REC_GPS_INIT_CLOSE_AGPS,
	
}Send_GPS_Init_Data_Index;

typedef enum
{
	SEND_GPS_DATA,
	REC_GPS_DATA,
	GPS_IDLE_MODE,
	
}Send_GPS_Data_Index;

/*********************************协议*************************************************************/
#define	BASE_DATA_MARK 					0x71		//电折水基础数据标识
#define	BASE_DATA_VER						3			//基础数据版本
	
#define	TRS_DATA_MARK						0x72		//电折水交易数据标识
#define	TRS_DATA_VER						3			//交易数据版本
	
#define 	FAULT_DATA_MARK					0x61		//故障柜标识
#define	FAULT_DATA_VER					2			//故障柜版本
	
#define	ALARM_DATA_MARK					0x68		//报警柜标识
#define 	ALARM_DATA_NO_VER				2			// 报警柜数据版本，无越限值
#define 	ALARM_DATA_XIANZHI_VER			3			// 报警柜数据版本，有越限值

#define	EVENT_DATA_MARK					0x6A		//事件柜
#define	EVENT_DATA_NO_VER				2			// 报警柜数据版本，无越限值
#define 	EVENT_DATA_XIANZHI_VER			3			// 报警柜数据版本，有越限值

#define	SET_BLACKLIST_CARD				0x5B
#define	SET_PROBLEM_LIST	 				0x5A
#define	SET_DEVICE_PARAM					0x55
#define	SET_RUNPLAN_PARAM				0x56
#define	SET_WATER_PRICE_PARAM			0x57
#define	SET_TIME_PARAM					0x58
#define	SET_RESET_PRICE_TIME				0x5C
#define	SET_USER_PRICE_GRADE				0x5D
#define	SET_DOWN_CASUAL_TIME			0x1A
#define	SET_DOWN_CASUAL_STATE			0x19

#define	SET_OUTWATER_PARAM				0x59
// -- 终端类别
#define DEVICE_CODE		0xA4			
// --------------------------------------------------
#define Upload_Data_CODE_ZHAOCE				0x20			// 召测数据
#define Upload_Data_CODE_DINGSHI				0x21			// 定时上报数据
#define Upload_Data_CODE_BAOJING				0x22			// 报警数据
#define Upload_Data_CODE_GUZHANG				0x23			// 故障数据
#define Upload_Data_CODE_JIAOYI					0x24			// 交易数据
#define Upload_Data_CODE_JIAOYI_OFFLINE			0x25			// 交易数据
#define Upload_Data_CODE_SHIJIAN				0x26			// 事件数据
// --------------------------------------------------
// 上报数据版本
#define DATA_MASK_VERSION						0x23			// 数据版本
#define GUZHANGGUI_DATA_VER2					2				// 故障柜数据版本
#define GUZHANGGUI_DATA_VER3					3				// 故障柜数据版本
#define BAOJINGGUI_DATA_NO_VER				2				// 报警柜数据版本，无越限值
#define BAOJINGGUI_DATA_XIANZHI_VER			3				// 报警柜数据版本，有越限值
#define BANBENGUI_DATA_VER					2				// 版本柜数据版本

//----上位机通讯指令
// order mask 
#define ORDER_BASE_LENGH 						 20	// 命令基本长度

//水资源协议
#define WATER_ORDER_START_CODE			0x68	//水资源开始
#define WATER_ORDER_AFN					0xD1	//水资源AFN
#define WATER_ORDER_CONTROL_UP			0x80	//控制域上行
#define WATER_ORDER_CONTROL_DOWN		0		//控制域下行
#define WATER_ORDER_END_CODE			0x16	//水资源协议结束

//宏电协议
#define HD_ORDER_START_CODE				0x7B	//宏电协议起始
#define HD_ORDER_TYPE_CODE				9		//宏电协议包类型
#define HD_ORDER_LENGTH_CODE				0x10	//宏电协议长度
#define HD_ORDER_END_CODE				0x7B	//宏电协议起始

//德通协议
#define ORDER_START_CODE					0x3C	// 命令起始符
#define ORDER_VERSION						0x23	// 命令版本号
#define ORDER_DATA_VERSION				0x22	// 版本版本(22-带BCD时间   23-无时间)
#define ORDER_CODE							0xA1	// 命令标识码

#define ORDER_STATION_LENGTH				17		// 终端编码长度

#define ORDER_DIR_MD_SEND					1		// 命令方向 - 上位机发起
#define ORDER_DIR_TM_RESPOND				2		// 命令方向 - 终端应答
#define ORDER_DIR_TM_SEND					3		// 命令方向 - 终端发起
#define ORDER_DIR_MD_RESPOND				4		// 命令方向 - 上位机应答

#define ORDER_END_CODE					0x3e	// 命令结束符

// ----------------------------------------------------
// order code
#define ORDER_CODE_WRITE_PARAM					0		// 下发一个参数
#define ORDER_CODE_WRITE_PARAM_VER				2		// 命令版本		小数据

#define ORDER_CODE_READ_PARAM					1		// 读一个参数
#define ORDER_CODE_READ_PARAM_VER				2		// 命令版本

#define ORDER_CODE_WRITE_MULTI_PARAM			0x06	// 写多个参数
#define ORDER_CODE_WRITE_MULTI_PARAM_VER		2		// 命令版本

#define ORDER_CODE_SET_STATE						2		// 设定状态
#define ORDER_CODE_SET_STATE_VER					2		// 命令版本

#define ORDER_CODE_RESET_TERMINAL				0x02	//终端复位
#define ORDER_CODE_RESET_TERMINAL_VER			0x02	//终端复位指令版本

#define ORDER_CODE_INQUIRY						0x21	// 召测数据
#define ORDER_CODE_INQUIRY_VER					0x03	// 召测数据指令版本

#define ORDER_CODE_TIMING							0x22	// 校时
#define ORDER_CODE_TIMING_VER					0x03	// 校时指令版本

#define ORDER_CODE_INQUIRY_HISTORY				0x25	// 召测历史数据
#define ORDER_CODE_INQUIRY_HISTORY_VER			0x03	// 召测历史数据指令版本

#define MAIN_CONTER_MARK							0x01	// 主上位系统标识

#define ORDER_CODE_TIMED_REPORT					0x80	// 定时上报
#define ORDER_CODE_NOTIMED_REPORT_VER			0x02	// 定时上报指令版本    02-应答结果  03-应答结果+上位机ID+时间
#define ORDER_CODE_TIMED_REPORT_VER				0x03	// 定时上报指令版本    02-应答结果  03-应答结果+上位机ID+时间

#define ORDER_CODE_REQUEST_TIMING				0x83	// 终端请求校时
#define ORDER_CODE_REQUEST_TIMING_VER			0x02	// 终端请求校时指令版本
// -----------------------------------------------------
// 操作设备
#define ORDER_WORK_DEVICE_OWN					0x00	// 操作本机

#define ORDER_WORK_DEVICE_VALVE					0xB1	// 操作阀门

#define ORDER_WORK_TINGZHI						0		// 停止
#define ORDER_WORK_QIDONG						1		// 启动
#define ORDER_WORK_FUWEI							2		// 复位	
#define ORDER_WORK_QIECHU						0x10	// 切除
#define ORDER_WORK_TOURU							0x11	// 投入

#define ORDER_WORK_PARA_ONOFF					0x60	// 下发启用禁用
#define ORDER_WORK_PARA_YALI						0x30	// 下发压力
#define ORDER_WORK_PARA_STATION_NUMBER		0x10	// 下发终端编号
#define ORDER_WORK_PARA_SHANGBAO_JIANGE		0x11	// 下发上报间隔

#define ORDER_WORK_PARA_SYSCFG					0x55	//下发系统参数
#define ORDER_WORK_PARA_IRRIGATION_PLAN		0x58	//下发轮灌计划

#define ORDER_WORK_PARA_DTU_NUMBER				0xB2	// 下发DTU
// --------------------------------------------------
// -- 返回操作代码
#define COMMU_RESULT_SUCCEED				1		// 成功
#define COMMU_RESULT_FAIL					0		// 失败
#define COMMU_ERROR_CRC					0x10	// CRC 错
#define COMMU_ERROR_CS					0x12	// CS 错
#define COMMU_ERROR_LENGTH				0x11	// 长度错
#define COMMU_ERROR_NONSUPORT			0x13	// 不支持指令
#define COMMU_ERROR_FORM					0x14	// 参数格式错
#define COMMU_ERROR_SNERR				0x16	// 注册码不匹配
#define COMMU_ERROR_NOFILE				0x17	// 数据不存在
/**********************************************************************************************/

//命令字
#define ORD_WRITE 			0
#define ORD_READ			1

//返回状态
#define SUCCESSED_FLAG     	91
#define NORMAL_FLAG		92
#define ERROR_FLAG			93
#define TIMEOUT_FLAG		94
#define LEN_ERROR_FLAG		95
#define OFFLINE_FLAG		96
#define CRC_ERROR_FLAG		97
#define ADD_ERROR_FLAG		98


//定时时间(10ms)
#define TIME10MS        	1    
#define TIME20MS        	2    
#define TIME30MS        	3    
#define TIME40MS        	4    
#define TIME50MS        	5    
#define TIME100MS    	 	10    
#define TIME150MS    	 	15  
#define TIME200MS     	20  
#define TIME250MS     	25  
#define TIME300MS      	30  
#define TIME350MS       	35  
#define TIME400MS       	40  
#define TIME500MS       	50    
#define TIME600MS 		60    
#define TIME700MS 		70    
#define TIME800MS 		80    
#define TIME1S      		(100*TIME10MS)    
#define TIME10S    		(10*TIME1S)   
#define TIME1MIN   		(60*TIME1S)  
#define TIME10MIN  		(10*TIME1MIN) 
#define TIME1HOUR 		(60*TIME1MIN) 
#define TIME2HOUR 		(2*TIME1HOUR)

#endif

