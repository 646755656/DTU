#ifndef __GPIO_H
#define  __GPIO_H


//¸´Î»
#define DTU_RST_PORT 	GPIOA
#define DTU_RST_PIN 	GPIO_Pin_8
#define DTU_RST_OUT 	PAout(8)

#define PWR_KEY_PORT 	GPIOB
#define PWR_KEY_PIN 	GPIO_Pin_1
#define PWR_KEY_OUT 	PBout(1)

void Gpio_Init(void);

#endif

