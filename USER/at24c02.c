//模拟I2c 端口

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"

#include "watchdog.h"
#include "at24c02.h"
#include "sys.h"
#include "delay.h"



/* *************************************************************
 --EEPROM的操作函数
 --24CXX驱动函数
**************************************************************************
    - 功能描述：STM32f103  EEPORM初始化函数
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：无
    - 函数功能：实现I2C的初始化。
***************************************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();
}


/***************************************************************************
    - 功能描述：STM32f103 模拟IIC初始化函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：无
    - 函数实现步骤：
		（1) 配置好模拟IIC的PC11和PC12的引脚状态
		（2）并打开时钟
		（3）将SCL和SDA拉为高电平，使总线处于空闲状态
***************************************************************************/
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(IIC_SCL_CLK | IIC_SDA_CLK, ENABLE );	
	//打开GPIOC的时钟					 
	   
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);
	//配置SCL,也就是PC1为推挽输出模式

	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
	//配置SDA,也就是PC2为浮空输入模式

//	GPIO_InitStructure.GPIO_Pin = IIC_WP_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(IIC_WP_PORT, &GPIO_InitStructure);
	//配置WP,也就是PC0为推挽输出模式

//	Clr_IIC_WP;
	Set_IIC_SCL;
	Set_IIC_SDA;
	//将SCL和SDA都拉为高电平，表示IIC总线处于高电平
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(IIC_SDA_CLK, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(IIC_SDA_CLK, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

/***************************************************************************
    - 功能描述：STM32f103 模拟IIC起始信号函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：无
    - 函数实现步骤：
		（1）首先将SDA配置为输出
		（2）在SCL为高电平期间，SDA有下降沿的变化
***************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	Set_IIC_SDA;	  	  
	Set_IIC_SCL;
	delay_us(4);
	Clr_IIC_SDA;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	Clr_IIC_SCL;//钳住I2C总线，准备发送或接收数据 
}	  
/***************************************************************************
    - 功能描述：STM32f103 模拟IIC终止信号函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：无
    - 函数实现步骤：
		（1）首先将SDA配置为输出
		（2）在SCL为高电平期间，SDA有上升沿的变化
***************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	Clr_IIC_SCL;
	Clr_IIC_SDA;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	Set_IIC_SCL; 
	Set_IIC_SDA;//发送I2C总线结束信号
	delay_us(4);							   	
}

/***************************************************************************
    - 功能描述：STM32f103 模拟IIC等待应答函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无		
 	- 返回说明：返回值：1，接收应答失败；0，接收应答成功
    - 函数说明：
		（1）首先将SDA配置为输入
		（2）如果返回0，表示接收应答成功；如果为1，接收应答失败
		（3）注意在IIC_STOP()中	,将SDA置为了输出，所以再满足250条件后，产中IIC停止线号，
			 对输出线SDA读SDA输入值是为0的，所以才退出循环。
***************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	Set_IIC_SDA;
	delay_us(1);	   
	Set_IIC_SCL;
	delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	Clr_IIC_SCL;//时钟输出0 	   
	return 0;  
} 
/***************************************************************************
    - 功能描述：STM32f103 模拟IIC应答信号函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：无
    - 函数实现步骤：
		（1）首先将SDA配置为输出
		（2）在SCL的一个时钟周期期间，SDA为低 
***************************************************************************/
//产生ACK应答
void IIC_Ack(void)
{
	Clr_IIC_SCL;
	SDA_OUT();
	Clr_IIC_SDA;
	delay_us(2);
	Set_IIC_SCL;
	delay_us(2);
	Clr_IIC_SCL;
}
/***************************************************************************
    - 功能描述：STM32f103 模拟IIC不应答信号函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：无
    - 函数实现步骤：
		（1）首先将SDA配置为输出
		（2）在SCL的一个时钟周期期间，SDA为高电平
***************************************************************************/	    
void IIC_NAck(void)
{
	Clr_IIC_SCL;
	SDA_OUT();
	Set_IIC_SDA;
	delay_us(2);
	Set_IIC_SCL;
	delay_us(2);
	Clr_IIC_SCL;
}					 				     
/***************************************************************************
    - 功能描述：STM32f103 模拟IIC 发送一个字节函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：无
    - 函数实现步骤：
		（1）首先将SDA配置为输出
		（2）在SCL为低电平期间，置位SDA的数据为0或者1，延时后拉高SCL,取走数据，循环8次
***************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    Clr_IIC_SCL;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if ((txd&0x80)>>7) 	
			Set_IIC_SDA;
		else Clr_IIC_SDA;
        	txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		Set_IIC_SCL;
		delay_us(2); 
		Clr_IIC_SCL;	
		delay_us(2);
    }	 
} 

/***************************************************************************
    - 功能描述：STM32f103 模拟IIC 发送一个字节函数
    - 隶属模块：STM32 IIC操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：ack 用于决定是是否回应ACK
 	- 返回说明：无
    - 函数实现步骤：
		（1）首先将SDA配置为输入
		（2）在SCL的一个时钟周期后，读SDA的数据，保存在receive中
		（3）读1个字节，ack=1时，发送ACK，ack=0，发送nACK  

***************************************************************************/	     
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    	for(i=0;i<8;i++ )
	{
	        Clr_IIC_SCL; 
	        delay_us(2);
			Set_IIC_SCL;
	        receive<<=1;
	        if(READ_SDA)receive++;   
			delay_us(1); 
	}					 
	if (!ack)
		IIC_NAck();//发送nACK
	else
		IIC_Ack(); //发送ACK   
	return receive;
}
/***************************************************************************
    - 功能描述：STM32f103  任意读取EEPORM一个字节的函数
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：ReadAddr为所读取EEPROM字节的地址
 	- 返回说明：无
    - 函数功能：返回所要读取的地址的数据
	- 函数说明：要任意读取EEPROM地址中的一个字节的步骤为:
	  (1)发送开始START信号
	  (2)发送器件器件地址，最后一位为0
	  (3)从机检测到主机发送的地址与自己的地址相同时发送一个应答信号ACK
	  (4)发送待操作的字节地址
	  (5)等待一个ACK
	  (6)重新发送START起始信号
	  (7)发送7位器件地址，最后一位为1，表示读EEPROM。
	  (8)等待ACK，读取一个字节。
	  (9)I2C停止，返回读取的数据。
***************************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

/***************************************************************************
    - 功能描述：STM32f103  任意写EEPORM一个字节的函数
    -地址:1010为固定，A0，A1，A2正好与芯片的1，2，3引角对应
    		最后一位R/W为告诉从机下一字节数据是要读还是写，0为写入，1为读出。
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：WriteAddr  :写入数据的目的地址 ，DataToWrite:要写入的数据
 	- 返回说明：无
    - 函数功能：返回所要读取的地址的数据
	- 函数说明：要任意读取EEPROM地址中的一个字节的步骤为:
	  (1)发送开始START信号
	  (2)发送器件器件地址，最后一位为0
	  (3)从机检测到主机发送的地址与自己的地址相同时发送一个应答信号ACK
	  (4)发送待操作的字节地址
	  (5)等待一个ACK
	  (6)发送字节数据
	  (7)等待一个ACK
	  (8)I2C停止，返回读取的数据。
***************************************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();						//产生一个停止条件 
	delay_ms(10);	 				//EEPROM的写入速度比较慢，加入延迟
}


/***************************************************************************
    - 功能描述：STM32f103  任意写EEPORM指定地址开始写入长度为Len的数据
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：WriteAddr  :写入数据的目的地址 ，DataToWrite:数据数组首地址
				Len:要写入数据的长度2,4
 	- 返回说明：无
    - 函数功能：无
	- 函数说明：就是调用len次 AT24CXX_WriteOneByte函数，该函数用于写入16bit或者32bit的数据.
***************************************************************************/
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}
/***************************************************************************
    - 功能描述：STM32f103  任意写EEPORM指定地址开始读出长度为Len的数据
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：ReadAddr   :开始读出的地址
				Len:要写入数据的长度2,4
 	- 返回说明：无
    - 函数功能：无
	- 函数说明：就是调用len次AT24CXX_ReadOneByte函数，该函数用于读出16bit或者32bit的数据.
***************************************************************************/
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
/***************************************************************************
    - 功能描述：STM32f103检查AT24CXX是否正常
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：无
 	- 返回说明：返回1:检测失败 返回0:检测成功
    - 函数功能：无
	- 函数说明：这里用了24XX的最后一个地址(255)来存储标志字.
***************************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    	temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}


/***************************************************************************
    - 功能描述：STM32f103在AT24CXX里面的指定地址开始读出指定个数的数据
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：ReadAddr :开始读出的地址 对24c02为0~255
				pBuffer  :数据数组首地址
				NumToRead:要读出数据的个数
 	- 返回说明：无
    - 函数功能：无
***************************************************************************/
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	unsigned char i=0;
	while(NumToRead)
	{
		pBuffer[i++]=AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
		WatchDog();
	}
	/*
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
	*/
}  

/***************************************************************************
    - 功能描述：STM32f103在AT24CXX里面的指定地址开始写入指定个数的数据
    - 隶属模块：STM32 EEPROM操作
 	- 函数属性：外部，使用户使用
 	- 参数说明：ReadAddr :开始读出的地址 对24c02为0~255
				pBuffer  :数据数组首地址，所读到的数据存储在了pBuffer中
				NumToRead:要读出数据的个数
 	- 返回说明：无
    - 函数功能：无
***************************************************************************/

void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	unsigned char i=0;
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,pBuffer[i]);
		WriteAddr++;
		i++;
		WatchDog();
	}
	/*
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
	*/
}
 
//**************************************************************/
