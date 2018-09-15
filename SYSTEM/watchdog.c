//#include "stm32f10x_lib.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
#include "watchdog.h"

u16 Reload = 349;
//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).	
void WDG_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	IWDG_SetReload(Reload);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void WatchDog()
{
	IWDG_ReloadCounter();
}


