#ifndef __GLOBAL_H
#define __GLOBAL_H

typedef enum
{
	GET_4G_MODULE_GPS,	//��ȡGPS
	NET_INIT,				//�����ʼ��
	CONNECT_SERVER,		//���ӷ�����
	TCP1_CONNECT,			//����TCP
	TCP2_CONNECT,			//����TCP
	REC_TERMINAL_DATA,	//�����ն˷��͵�����
	SEND_REC_DATA,		//���ͽ��յ�����
	ERROR_RUN_STATUS,	//û��IP�Ͷ˿�
	
}Net_Index;

typedef enum
{
	SEND_INIT_IDLE_MODE,
		
	SEND_INIT_TEST_UART,							//ȷ�ϴ�������
	SEND_INIT_ENTER_PIN,					//��ѯSIM��״̬				
	SEND_INIT_SIGNAL_QUALITY,				//��ѯ�ź�ǿ��			
	SEND_INIT_REG_STATUS,					//ע�ᵽ����
	SEND_INIT_GPRS_STATUS,				//GPRS����ע��״̬	
//	SEND_INIT_NETWORK_ATT,					//���GPRS����״̬

	REC_INIT_TEST_UART,	
	REC_INIT_ENTER_PIN,							
	REC_INIT_SIGNAL_QUALITY,	
	REC_INIT_REG_STATUS,			
	REC_INIT_GPRS_STATUS,					//GPRS����ע��״̬	
//	REC_INIT_NETWORK_ATT,					//���GPRS����״̬
	
	INIT_FUNCTION_SELECT,
}Net_Init_Index;

typedef enum
{
	SEND_PDP_IDLE_MODE,
		
	SEND_PDP_APN_SETTING,					//APN�趨
	SEND_PDP_PROFILE_NUMBER,
	SEND_PDP_TCP_MODE,			//Select TCP/IP application mode
	SEND_PDP_NETOPEN,
	SEND_PDP_IPADDR,					//��ѯ��ǰ����socket PDP��IP��ַ
	

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
	SEND_TCP_ESTABLISH_CONNECT,		//TCP����
	SEND_TCP_INQUIRY_CONNECT,		//��ѯ�Ƿ�����
	
	REC_TCP_FIRST,
	REC_TCP_ESTABLISH_CONNECT,
	REC_TCP_INQUIRY_CONNECT,
}TCP_Client_Index;

typedef enum
{
	SEND_DATA_SET_CHANNEL,		//�趨��������ͨ��
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

/*********************************Э��*************************************************************/
#define	BASE_DATA_MARK 					0x71		//����ˮ�������ݱ�ʶ
#define	BASE_DATA_VER						3			//�������ݰ汾
	
#define	TRS_DATA_MARK						0x72		//����ˮ�������ݱ�ʶ
#define	TRS_DATA_VER						3			//�������ݰ汾
	
#define 	FAULT_DATA_MARK					0x61		//���Ϲ��ʶ
#define	FAULT_DATA_VER					2			//���Ϲ�汾
	
#define	ALARM_DATA_MARK					0x68		//�������ʶ
#define 	ALARM_DATA_NO_VER				2			// ���������ݰ汾����Խ��ֵ
#define 	ALARM_DATA_XIANZHI_VER			3			// ���������ݰ汾����Խ��ֵ

#define	EVENT_DATA_MARK					0x6A		//�¼���
#define	EVENT_DATA_NO_VER				2			// ���������ݰ汾����Խ��ֵ
#define 	EVENT_DATA_XIANZHI_VER			3			// ���������ݰ汾����Խ��ֵ

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
// -- �ն����
#define DEVICE_CODE		0xA4			
// --------------------------------------------------
#define Upload_Data_CODE_ZHAOCE				0x20			// �ٲ�����
#define Upload_Data_CODE_DINGSHI				0x21			// ��ʱ�ϱ�����
#define Upload_Data_CODE_BAOJING				0x22			// ��������
#define Upload_Data_CODE_GUZHANG				0x23			// ��������
#define Upload_Data_CODE_JIAOYI					0x24			// ��������
#define Upload_Data_CODE_JIAOYI_OFFLINE			0x25			// ��������
#define Upload_Data_CODE_SHIJIAN				0x26			// �¼�����
// --------------------------------------------------
// �ϱ����ݰ汾
#define DATA_MASK_VERSION						0x23			// ���ݰ汾
#define GUZHANGGUI_DATA_VER2					2				// ���Ϲ����ݰ汾
#define GUZHANGGUI_DATA_VER3					3				// ���Ϲ����ݰ汾
#define BAOJINGGUI_DATA_NO_VER				2				// ���������ݰ汾����Խ��ֵ
#define BAOJINGGUI_DATA_XIANZHI_VER			3				// ���������ݰ汾����Խ��ֵ
#define BANBENGUI_DATA_VER					2				// �汾�����ݰ汾

//----��λ��ͨѶָ��
// order mask 
#define ORDER_BASE_LENGH 						 20	// �����������

//ˮ��ԴЭ��
#define WATER_ORDER_START_CODE			0x68	//ˮ��Դ��ʼ
#define WATER_ORDER_AFN					0xD1	//ˮ��ԴAFN
#define WATER_ORDER_CONTROL_UP			0x80	//����������
#define WATER_ORDER_CONTROL_DOWN		0		//����������
#define WATER_ORDER_END_CODE			0x16	//ˮ��ԴЭ�����

//���Э��
#define HD_ORDER_START_CODE				0x7B	//���Э����ʼ
#define HD_ORDER_TYPE_CODE				9		//���Э�������
#define HD_ORDER_LENGTH_CODE				0x10	//���Э�鳤��
#define HD_ORDER_END_CODE				0x7B	//���Э����ʼ

//��ͨЭ��
#define ORDER_START_CODE					0x3C	// ������ʼ��
#define ORDER_VERSION						0x23	// ����汾��
#define ORDER_DATA_VERSION				0x22	// �汾�汾(22-��BCDʱ��   23-��ʱ��)
#define ORDER_CODE							0xA1	// �����ʶ��

#define ORDER_STATION_LENGTH				17		// �ն˱��볤��

#define ORDER_DIR_MD_SEND					1		// ����� - ��λ������
#define ORDER_DIR_TM_RESPOND				2		// ����� - �ն�Ӧ��
#define ORDER_DIR_TM_SEND					3		// ����� - �ն˷���
#define ORDER_DIR_MD_RESPOND				4		// ����� - ��λ��Ӧ��

#define ORDER_END_CODE					0x3e	// ���������

// ----------------------------------------------------
// order code
#define ORDER_CODE_WRITE_PARAM					0		// �·�һ������
#define ORDER_CODE_WRITE_PARAM_VER				2		// ����汾		С����

#define ORDER_CODE_READ_PARAM					1		// ��һ������
#define ORDER_CODE_READ_PARAM_VER				2		// ����汾

#define ORDER_CODE_WRITE_MULTI_PARAM			0x06	// д�������
#define ORDER_CODE_WRITE_MULTI_PARAM_VER		2		// ����汾

#define ORDER_CODE_SET_STATE						2		// �趨״̬
#define ORDER_CODE_SET_STATE_VER					2		// ����汾

#define ORDER_CODE_RESET_TERMINAL				0x02	//�ն˸�λ
#define ORDER_CODE_RESET_TERMINAL_VER			0x02	//�ն˸�λָ��汾

#define ORDER_CODE_INQUIRY						0x21	// �ٲ�����
#define ORDER_CODE_INQUIRY_VER					0x03	// �ٲ�����ָ��汾

#define ORDER_CODE_TIMING							0x22	// Уʱ
#define ORDER_CODE_TIMING_VER					0x03	// Уʱָ��汾

#define ORDER_CODE_INQUIRY_HISTORY				0x25	// �ٲ���ʷ����
#define ORDER_CODE_INQUIRY_HISTORY_VER			0x03	// �ٲ���ʷ����ָ��汾

#define MAIN_CONTER_MARK							0x01	// ����λϵͳ��ʶ

#define ORDER_CODE_TIMED_REPORT					0x80	// ��ʱ�ϱ�
#define ORDER_CODE_NOTIMED_REPORT_VER			0x02	// ��ʱ�ϱ�ָ��汾    02-Ӧ����  03-Ӧ����+��λ��ID+ʱ��
#define ORDER_CODE_TIMED_REPORT_VER				0x03	// ��ʱ�ϱ�ָ��汾    02-Ӧ����  03-Ӧ����+��λ��ID+ʱ��

#define ORDER_CODE_REQUEST_TIMING				0x83	// �ն�����Уʱ
#define ORDER_CODE_REQUEST_TIMING_VER			0x02	// �ն�����Уʱָ��汾
// -----------------------------------------------------
// �����豸
#define ORDER_WORK_DEVICE_OWN					0x00	// ��������

#define ORDER_WORK_DEVICE_VALVE					0xB1	// ��������

#define ORDER_WORK_TINGZHI						0		// ֹͣ
#define ORDER_WORK_QIDONG						1		// ����
#define ORDER_WORK_FUWEI							2		// ��λ	
#define ORDER_WORK_QIECHU						0x10	// �г�
#define ORDER_WORK_TOURU							0x11	// Ͷ��

#define ORDER_WORK_PARA_ONOFF					0x60	// �·����ý���
#define ORDER_WORK_PARA_YALI						0x30	// �·�ѹ��
#define ORDER_WORK_PARA_STATION_NUMBER		0x10	// �·��ն˱��
#define ORDER_WORK_PARA_SHANGBAO_JIANGE		0x11	// �·��ϱ����

#define ORDER_WORK_PARA_SYSCFG					0x55	//�·�ϵͳ����
#define ORDER_WORK_PARA_IRRIGATION_PLAN		0x58	//�·��ֹ�ƻ�

#define ORDER_WORK_PARA_DTU_NUMBER				0xB2	// �·�DTU
// --------------------------------------------------
// -- ���ز�������
#define COMMU_RESULT_SUCCEED				1		// �ɹ�
#define COMMU_RESULT_FAIL					0		// ʧ��
#define COMMU_ERROR_CRC					0x10	// CRC ��
#define COMMU_ERROR_CS					0x12	// CS ��
#define COMMU_ERROR_LENGTH				0x11	// ���ȴ�
#define COMMU_ERROR_NONSUPORT			0x13	// ��֧��ָ��
#define COMMU_ERROR_FORM					0x14	// ������ʽ��
#define COMMU_ERROR_SNERR				0x16	// ע���벻ƥ��
#define COMMU_ERROR_NOFILE				0x17	// ���ݲ�����
/**********************************************************************************************/

//������
#define ORD_WRITE 			0
#define ORD_READ			1

//����״̬
#define SUCCESSED_FLAG     	91
#define NORMAL_FLAG		92
#define ERROR_FLAG			93
#define TIMEOUT_FLAG		94
#define LEN_ERROR_FLAG		95
#define OFFLINE_FLAG		96
#define CRC_ERROR_FLAG		97
#define ADD_ERROR_FLAG		98


//��ʱʱ��(10ms)
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

