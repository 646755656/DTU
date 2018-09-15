//#include "stm32f10x_lib.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
#include "watchdog.h"

u16 Reload = 349;
//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).	
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


