//ģ��I2c �˿�

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"

#include "watchdog.h"
#include "at24c02.h"
#include "sys.h"
#include "delay.h"



/* *************************************************************
 --EEPROM�Ĳ�������
 --24CXX��������
**************************************************************************
    - ����������STM32f103  EEPORM��ʼ������
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵������
    - �������ܣ�ʵ��I2C�ĳ�ʼ����
***************************************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();
}


/***************************************************************************
    - ����������STM32f103 ģ��IIC��ʼ������
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵������
    - ����ʵ�ֲ��裺
		��1) ���ú�ģ��IIC��PC11��PC12������״̬
		��2������ʱ��
		��3����SCL��SDA��Ϊ�ߵ�ƽ��ʹ���ߴ��ڿ���״̬
***************************************************************************/
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(IIC_SCL_CLK | IIC_SDA_CLK, ENABLE );	
	//��GPIOC��ʱ��					 
	   
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);
	//����SCL,Ҳ����PC1Ϊ�������ģʽ

	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
	//����SDA,Ҳ����PC2Ϊ��������ģʽ

//	GPIO_InitStructure.GPIO_Pin = IIC_WP_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(IIC_WP_PORT, &GPIO_InitStructure);
	//����WP,Ҳ����PC0Ϊ�������ģʽ

//	Clr_IIC_WP;
	Set_IIC_SCL;
	Set_IIC_SDA;
	//��SCL��SDA����Ϊ�ߵ�ƽ����ʾIIC���ߴ��ڸߵ�ƽ
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
    - ����������STM32f103 ģ��IIC��ʼ�źź���
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵������
    - ����ʵ�ֲ��裺
		��1�����Ƚ�SDA����Ϊ���
		��2����SCLΪ�ߵ�ƽ�ڼ䣬SDA���½��صı仯
***************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	Set_IIC_SDA;	  	  
	Set_IIC_SCL;
	delay_us(4);
	Clr_IIC_SDA;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	Clr_IIC_SCL;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
/***************************************************************************
    - ����������STM32f103 ģ��IIC��ֹ�źź���
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵������
    - ����ʵ�ֲ��裺
		��1�����Ƚ�SDA����Ϊ���
		��2����SCLΪ�ߵ�ƽ�ڼ䣬SDA�������صı仯
***************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	Clr_IIC_SCL;
	Clr_IIC_SDA;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	Set_IIC_SCL; 
	Set_IIC_SDA;//����I2C���߽����ź�
	delay_us(4);							   	
}

/***************************************************************************
    - ����������STM32f103 ģ��IIC�ȴ�Ӧ����
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������		
 	- ����˵��������ֵ��1������Ӧ��ʧ�ܣ�0������Ӧ��ɹ�
    - ����˵����
		��1�����Ƚ�SDA����Ϊ����
		��2���������0����ʾ����Ӧ��ɹ������Ϊ1������Ӧ��ʧ��
		��3��ע����IIC_STOP()��	,��SDA��Ϊ�����������������250�����󣬲���IICֹͣ�ߺţ�
			 �������SDA��SDA����ֵ��Ϊ0�ģ����Բ��˳�ѭ����
***************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	Clr_IIC_SCL;//ʱ�����0 	   
	return 0;  
} 
/***************************************************************************
    - ����������STM32f103 ģ��IICӦ���źź���
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵������
    - ����ʵ�ֲ��裺
		��1�����Ƚ�SDA����Ϊ���
		��2����SCL��һ��ʱ�������ڼ䣬SDAΪ�� 
***************************************************************************/
//����ACKӦ��
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
    - ����������STM32f103 ģ��IIC��Ӧ���źź���
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵������
    - ����ʵ�ֲ��裺
		��1�����Ƚ�SDA����Ϊ���
		��2����SCL��һ��ʱ�������ڼ䣬SDAΪ�ߵ�ƽ
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
    - ����������STM32f103 ģ��IIC ����һ���ֽں���
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵������
    - ����ʵ�ֲ��裺
		��1�����Ƚ�SDA����Ϊ���
		��2����SCLΪ�͵�ƽ�ڼ䣬��λSDA������Ϊ0����1����ʱ������SCL,ȡ�����ݣ�ѭ��8��
***************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    Clr_IIC_SCL;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if ((txd&0x80)>>7) 	
			Set_IIC_SDA;
		else Clr_IIC_SDA;
        	txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		Set_IIC_SCL;
		delay_us(2); 
		Clr_IIC_SCL;	
		delay_us(2);
    }	 
} 

/***************************************************************************
    - ����������STM32f103 ģ��IIC ����һ���ֽں���
    - ����ģ�飺STM32 IIC����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵����ack ���ھ������Ƿ��ӦACK
 	- ����˵������
    - ����ʵ�ֲ��裺
		��1�����Ƚ�SDA����Ϊ����
		��2����SCL��һ��ʱ�����ں󣬶�SDA�����ݣ�������receive��
		��3����1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK  

***************************************************************************/	     
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
		IIC_NAck();//����nACK
	else
		IIC_Ack(); //����ACK   
	return receive;
}
/***************************************************************************
    - ����������STM32f103  �����ȡEEPORMһ���ֽڵĺ���
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵����ReadAddrΪ����ȡEEPROM�ֽڵĵ�ַ
 	- ����˵������
    - �������ܣ�������Ҫ��ȡ�ĵ�ַ������
	- ����˵����Ҫ�����ȡEEPROM��ַ�е�һ���ֽڵĲ���Ϊ:
	  (1)���Ϳ�ʼSTART�ź�
	  (2)��������������ַ�����һλΪ0
	  (3)�ӻ���⵽�������͵ĵ�ַ���Լ��ĵ�ַ��ͬʱ����һ��Ӧ���ź�ACK
	  (4)���ʹ��������ֽڵ�ַ
	  (5)�ȴ�һ��ACK
	  (6)���·���START��ʼ�ź�
	  (7)����7λ������ַ�����һλΪ1����ʾ��EEPROM��
	  (8)�ȴ�ACK����ȡһ���ֽڡ�
	  (9)I2Cֹͣ�����ض�ȡ�����ݡ�
***************************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

/***************************************************************************
    - ����������STM32f103  ����дEEPORMһ���ֽڵĺ���
    -��ַ:1010Ϊ�̶���A0��A1��A2������оƬ��1��2��3���Ƕ�Ӧ
    		���һλR/WΪ���ߴӻ���һ�ֽ�������Ҫ������д��0Ϊд�룬1Ϊ������
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵����WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ ��DataToWrite:Ҫд�������
 	- ����˵������
    - �������ܣ�������Ҫ��ȡ�ĵ�ַ������
	- ����˵����Ҫ�����ȡEEPROM��ַ�е�һ���ֽڵĲ���Ϊ:
	  (1)���Ϳ�ʼSTART�ź�
	  (2)��������������ַ�����һλΪ0
	  (3)�ӻ���⵽�������͵ĵ�ַ���Լ��ĵ�ַ��ͬʱ����һ��Ӧ���ź�ACK
	  (4)���ʹ��������ֽڵ�ַ
	  (5)�ȴ�һ��ACK
	  (6)�����ֽ�����
	  (7)�ȴ�һ��ACK
	  (8)I2Cֹͣ�����ض�ȡ�����ݡ�
***************************************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();						//����һ��ֹͣ���� 
	delay_ms(10);	 				//EEPROM��д���ٶȱȽ����������ӳ�
}


/***************************************************************************
    - ����������STM32f103  ����дEEPORMָ����ַ��ʼд�볤��ΪLen������
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵����WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ ��DataToWrite:���������׵�ַ
				Len:Ҫд�����ݵĳ���2,4
 	- ����˵������
    - �������ܣ���
	- ����˵�������ǵ���len�� AT24CXX_WriteOneByte�������ú�������д��16bit����32bit������.
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
    - ����������STM32f103  ����дEEPORMָ����ַ��ʼ��������ΪLen������
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵����ReadAddr   :��ʼ�����ĵ�ַ
				Len:Ҫд�����ݵĳ���2,4
 	- ����˵������
    - �������ܣ���
	- ����˵�������ǵ���len��AT24CXX_ReadOneByte�������ú������ڶ���16bit����32bit������.
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
    - ����������STM32f103���AT24CXX�Ƿ�����
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵������
 	- ����˵��������1:���ʧ�� ����0:���ɹ�
    - �������ܣ���
	- ����˵������������24XX�����һ����ַ(255)���洢��־��.
***************************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    	temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}


/***************************************************************************
    - ����������STM32f103��AT24CXX�����ָ����ַ��ʼ����ָ������������
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵����ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
				pBuffer  :���������׵�ַ
				NumToRead:Ҫ�������ݵĸ���
 	- ����˵������
    - �������ܣ���
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
    - ����������STM32f103��AT24CXX�����ָ����ַ��ʼд��ָ������������
    - ����ģ�飺STM32 EEPROM����
 	- �������ԣ��ⲿ��ʹ�û�ʹ��
 	- ����˵����ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
				pBuffer  :���������׵�ַ�������������ݴ洢����pBuffer��
				NumToRead:Ҫ�������ݵĸ���
 	- ����˵������
    - �������ܣ���
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
