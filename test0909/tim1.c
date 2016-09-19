#include"stm32f10x.h"
#include"delay.h"
#include"motion.h"

extern uint8_t command;
extern uint8_t precommand;

void TIM1_CC_IRQHandler(void){
	//50ms
	USART_SendData(USART1,precommand);
	motion_select();
}
