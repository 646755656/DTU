#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_DeInit(TIM2);

	//--��ʱ��2���ã� 720��Ƶ��50ms�ж�һ�Σ����ϼ���  //1000  10ms
	//Tout= ((arr+1)*(psc+1))/Tclk
	TIM_TimeBaseStructure.TIM_Period = 999;//�Զ�����װ�ؼĴ���
	TIM_TimeBaseStructure.TIM_Prescaler = 719;//720��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	//--ʹ��TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStructure);
#if 0
	//TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_DeInit(TIM3);

	//--��ʱ��3���ã� 720��Ƶ��250ms�ж�һ�Σ����ϼ���  
	TIM_TimeBaseStructure.TIM_Period = 499;//�Զ�����װ�ؼĴ���
	TIM_TimeBaseStructure.TIM_Prescaler = 719;//72��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	//--ʹ��TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	NVIC_Init(&NVIC_InitStructure);
#endif
}


