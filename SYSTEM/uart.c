#include "stm32f10x_it.h"
#include "stm32f10x_usart.h"
#include "uart.h"
#include "watchdog.h"
#include "sys.h"

struct com1_define com1;
struct com2_define com2;
struct com3_define com3;

void UART_Init(void)
{
	USART1_Init(115200);		// 4G模块
	USART2_Init(115200);		//外设
	USART3_Init(9600);		//串口配置
	
}

void USART1_Init(unsigned int baut)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	USART_DeInit(USART1);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = TX1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(TX1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RX1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RX1_PORT, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = baut;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

}

void USART2_Init(u32 baut)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//-----------------------------------------------------------------------------------------------
	//    串口2参数初始化定义部分,串口1参数为9600 ， 8 ，1 ，N  接收中断方式
	//使能串口2时钟
	USART_DeInit(USART2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);
	//使能串口2中断，
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 定义UART2 TX (PA.02)脚为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = TX2_PIN;         //IO口的第九脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //IO口复用推挽输出
	GPIO_Init(TX2_PORT, &GPIO_InitStructure);            //初始化串口1输出IO口

	// 定义 USART2 Rx (PA.03)为悬空输入 
	GPIO_InitStructure.GPIO_Pin = RX2_PIN;           //IO口的第十脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//IO口悬空输入
	GPIO_Init(RX2_PORT, &GPIO_InitStructure);               //初始化串口1输入IO口

	USART_InitStructure.USART_BaudRate = baut; //设定传输速率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //设定传输数据位数
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    //设定停止位个数
	USART_InitStructure.USART_Parity = USART_Parity_No ;      //不用校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不用流量控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //使用接收和发送功能
  
	USART_Init(USART2, &USART_InitStructure);  //初始化串口2
  
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //使能串口1接收中断
  
	USART_Cmd(USART2, ENABLE);  //使能串口2
}

void USART3_Init(unsigned int baut)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_DeInit(USART3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = TX3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(TX3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RX3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RX3_PORT, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = baut;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART3,&USART_InitStructure);

	USART_Cmd(USART3, ENABLE);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


//*******************************************************************************
// 函数名称    : UART1_PutChar()
// 功能描述    : 串口1发送数据函数.
// 输入        : u8 ch 要发送的数据
// 输出        : None
// 返回        : None
//******************************************************************************
void UART1_PutChar(unsigned char ch)
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待数据发送完毕
}

//*******************************************************************************
// 函数名称    : UART1_PutString()
// 功能描述    : 串口1发送数据函数.
// 输入        : u8 *buf 要发送的数据,len发送数据的长度
// 输出        : None
// 返回        : None
//******************************************************************************
void UART1_PutString(unsigned char *buf, unsigned int len)
{
	unsigned int i = 0;
	for(i=0; i<len; i++)
	{
		UART1_PutChar(buf[i]);
		WatchDog();
	}
}

void UART1_SendStr(unsigned char *buf)
{
	unsigned int i = 0;
	while(buf[i] !=0)
	{
		UART1_PutChar(buf[i]);
		i++;
	}
}


void UART1_Clear(void)
{
	clearbuff(com1.rcv_buff, RCV1_BUFF_LENGH);
	com1.jg_time = 0;
	com1.rcv_len = 0;
}
//*******************************************************************************
// 函数名称    : UART2_PutChar()
// 功能描述    : 串口1发送数据函数.
// 输入        : u8 ch 要发送的数据
// 输出        : None
// 返回        : None
//******************************************************************************
void UART2_PutChar(unsigned char ch)
{
	USART_SendData(USART2, ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);//等待数据发送完毕
}

//*******************************************************************************
// 函数名称    : UART2_PutString()
// 功能描述    : 串口2发送数据函数.
// 输入        : u8 *buf 要发送的数据,len发送数据的长度
// 输出        : None
// 返回        : None
//******************************************************************************
void UART2_PutString(unsigned char *buf, unsigned int len)
{
	unsigned int i = 0;
	for(i=0; i<len; i++)
	{
		UART2_PutChar(buf[i]);
		WatchDog();
	}
}

void UART2_SendStr(unsigned char *buf)
{
	unsigned int i = 0;
	while(buf[i] !=0)
	{
		UART2_PutChar(buf[i]);
		i++;
	}
}

void UART2_Clear(void)
{
	clearbuff(com2.rcv_buff, RCV2_BUFF_LENGH);
	com2.jg_time = 0;
	com2.rcv_len = 0;
}

//*******************************************************************************
// 函数名称    : UART3_PutChar()
// 功能描述    : 串口1发送数据函数.
// 输入        : u8 ch 要发送的数据
// 输出        : None
// 返回        : None
//******************************************************************************
void UART3_PutChar(unsigned char ch)
{
	USART_SendData(USART3, ch);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//等待数据发送完毕
}

//*******************************************************************************
// 函数名称    : UART3_PutString()
// 功能描述    : 串口1发送数据函数.
// 输入        : u8 *buf 要发送的数据,len发送数据的长度
// 输出        : None
// 返回        : None
//******************************************************************************
void UART3_PutString(unsigned char *buf, unsigned int len)
{
	unsigned int i = 0;
	for(i=0; i<len; i++)
	{
		UART3_PutChar(buf[i]);
		WatchDog();
	}
}

void UART3_SendStr(unsigned char *buf)
{
	unsigned int i = 0;
	while(buf[i] !=0)
	{
		UART3_PutChar(buf[i]);
		i++;
	}
}

void UART3_Clear(void)
{
	clearbuff(com3.rcv_buff, RCV3_BUFF_LENGH);
	com3.jg_time = 0;
	com3.rcv_len = 0;
}

