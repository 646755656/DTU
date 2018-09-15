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
	USART1_Init(115200);		// 4Gģ��
	USART2_Init(115200);		//����
	USART3_Init(9600);		//��������
	
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
	//    ����2������ʼ�����岿��,����1����Ϊ9600 �� 8 ��1 ��N  �����жϷ�ʽ
	//ʹ�ܴ���2ʱ��
	USART_DeInit(USART2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);
	//ʹ�ܴ���2�жϣ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// ����UART2 TX (PA.02)��Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = TX2_PIN;         //IO�ڵĵھŽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //IO�ڸ����������
	GPIO_Init(TX2_PORT, &GPIO_InitStructure);            //��ʼ������1���IO��

	// ���� USART2 Rx (PA.03)Ϊ�������� 
	GPIO_InitStructure.GPIO_Pin = RX2_PIN;           //IO�ڵĵ�ʮ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//IO����������
	GPIO_Init(RX2_PORT, &GPIO_InitStructure);               //��ʼ������1����IO��

	USART_InitStructure.USART_BaudRate = baut; //�趨��������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�趨��������λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    //�趨ֹͣλ����
	USART_InitStructure.USART_Parity = USART_Parity_No ;      //����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //ʹ�ý��պͷ��͹���
  
	USART_Init(USART2, &USART_InitStructure);  //��ʼ������2
  
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //ʹ�ܴ���1�����ж�
  
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
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
// ��������    : UART1_PutChar()
// ��������    : ����1�������ݺ���.
// ����        : u8 ch Ҫ���͵�����
// ���        : None
// ����        : None
//******************************************************************************
void UART1_PutChar(unsigned char ch)
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//�ȴ����ݷ������
}

//*******************************************************************************
// ��������    : UART1_PutString()
// ��������    : ����1�������ݺ���.
// ����        : u8 *buf Ҫ���͵�����,len�������ݵĳ���
// ���        : None
// ����        : None
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
// ��������    : UART2_PutChar()
// ��������    : ����1�������ݺ���.
// ����        : u8 ch Ҫ���͵�����
// ���        : None
// ����        : None
//******************************************************************************
void UART2_PutChar(unsigned char ch)
{
	USART_SendData(USART2, ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);//�ȴ����ݷ������
}

//*******************************************************************************
// ��������    : UART2_PutString()
// ��������    : ����2�������ݺ���.
// ����        : u8 *buf Ҫ���͵�����,len�������ݵĳ���
// ���        : None
// ����        : None
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
// ��������    : UART3_PutChar()
// ��������    : ����1�������ݺ���.
// ����        : u8 ch Ҫ���͵�����
// ���        : None
// ����        : None
//******************************************************************************
void UART3_PutChar(unsigned char ch)
{
	USART_SendData(USART3, ch);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//�ȴ����ݷ������
}

//*******************************************************************************
// ��������    : UART3_PutString()
// ��������    : ����1�������ݺ���.
// ����        : u8 *buf Ҫ���͵�����,len�������ݵĳ���
// ���        : None
// ����        : None
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

