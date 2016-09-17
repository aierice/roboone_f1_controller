#include"stm32f10x.h"

uint32_t timingdelay;

void delay_Configuration(void){
	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}
}

void SysTick_Handler(void){
	if(timingdelay){
		timingdelay--;
	}
}

void tdelay(uint32_t nTime){
	timingdelay= nTime;
	while(timingdelay);
}
