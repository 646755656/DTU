 
#ifndef __AT24C02_H
#define __AT24C02_H


/***************************************************************
	- ˵�������²�����AT24Cxx��Ѱַ�ռ䣬C0x ,X ��ʾXK ��C01��ʾ1K
	- 127��ʾ2^7  1Kbit/8=128Byte
	- 255��ʾ2^8  2Kbit/8=256Byte
	- 512��ʾ2^9  4Kbit/8=512Byte	
	- 
***************************************************************/
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
/**************************************************************
	--����ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
**************************************************************/
#define EE_TYPE AT24C02




/*********************************************************
	- I2C �궨�壺
	- ���ģ��I2Cʱ�Ĺܽţ���PC1ģ��CLK����PC2ģ��SDA
	- ������Ӧ�õ�ʱ��Ҫ��GPIOC��ʱ��		  
*********************************************************/
//#define IIC_WP_PIN			GPIO_Pin_11
//#define IIC_WP_PORT		GPIOA
//#define IIC_WP_CLK               	RCC_APB2Periph_GPIOA 

#define IIC_SCL_PIN               	GPIO_Pin_6
#define IIC_SCL_PORT 		GPIOB
#define IIC_SCL_CLK               	RCC_APB2Periph_GPIOB 

#define IIC_SDA_PIN               	GPIO_Pin_7
#define IIC_SDA_PORT            GPIOB
#define IIC_SDA_CLK               RCC_APB2Periph_GPIOB  

/*********************************************************
	- I2C�궨�壺
	- SDA��������:
    -	SDA_IN(): �˴��ǽ�SDA����Ϊ����Ϊ����ģʽ
	-	SDA_OUT():�˴��ǽ�SDA_OUT����Ϊ�������ģʽ
	˵�������������õĿ���д������ȻҲ�����ÿ⺯����ʼ����
		  ��ΪIIC��SDA�ߣ���Ҫ��Ϊ����ҲҪ��Ϊ�����						  
*********************************************************/
   		   

//#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
//#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}


/*********************************************************
	- I2C�궨�壺
	- I2Cʱ�Ӳ���:
    - Set_IIC_SCL: �˴��ǽ�SCL��Ϊ�ߵ�ƽ
	- Clr_IIC_SCL: �˴��ǽ�SCL��Ϊ�͵�ƽ
	- ˵����ͨ���������궨��Ϳ���ģ��IIC��ʱ�ӵ�ƽSCL�ĸߵͱ仯��			  
*********************************************************/
#define Set_IIC_SCL  GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN)
#define Clr_IIC_SCL  GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN) 


/*********************************************************
	- I2C�궨�壺
	- I2C�����߲���:
    - Set_IIC_SDA: �˴��ǽ�SDA��Ϊ�ߵ�ƽ
	- Clr_IIC_SDA: �˴��ǽ�SDA��Ϊ�͵�ƽ
	- READ_SDA	 : �˴��Ƕ�ȡSDA���ϵ�λ����
	- ˵����ͨ���������궨��Ϳ���ģ��IIC��������SDA��ƽ�ĸߵͱ仯��			  
*********************************************************/
#define Set_IIC_SDA  GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN)
#define Clr_IIC_SDA  GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN) 
#define READ_SDA    GPIO_ReadInputDataBit(IIC_SDA_PORT, IIC_SDA_PIN)

	 
//#define Set_IIC_WP  	GPIO_SetBits(IIC_WP_PORT,IIC_WP_PIN)
//#define Clr_IIC_WP 	GPIO_ResetBits(IIC_WP_PORT,IIC_WP_PIN) 
/*********************************************************
	- I2Cģ�����������������	  
*********************************************************/
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void SDA_IN(void);
void SDA_OUT(void);
/*********************************************************
	- I2C�ⲿ����������������	  
*********************************************************/
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  



/**************************************************************
	--EEPROM�Ĳ�������
	--24CXX��������
**************************************************************/
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC
#endif

