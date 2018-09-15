
#include "stm32f10x_gpio.h"
#include "gpio.h"
#include "sys.h"

void Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
							 |RCC_APB2Periph_GPIOB
							 |RCC_APB2Periph_GPIOC
							 |RCC_APB2Periph_GPIOD
							 |RCC_APB2Periph_GPIOE
							 ,ENABLE);

	/***********RSTÒý½Å******************/
	GPIO_InitStructure.GPIO_Pin = DTU_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DTU_RST_PORT, &GPIO_InitStructure);   

	/***********PWRKEYÒý½Å******************/
	GPIO_InitStructure.GPIO_Pin = PWR_KEY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PWR_KEY_PORT,&GPIO_InitStructure);

}



