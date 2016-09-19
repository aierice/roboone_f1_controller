#include"stm32f10x.h"
#include"delay.h"

extern uint8_t precommand;
extern uint8_t command;

uint8_t switch_pattern(void){
	return 0;
}

void motion_select(void){
	uint8_t command_counter[3];
	uint8_t i,error_counter = 0;
	do{
	for(i=0; i<=2; i++){
		command_counter[i] = switch_pattern();
		tdelay(3);
	}
	error_counter++;
	if(error_counter >= 5){
		while(1)
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
	}
	}while(command_counter[0]==command_counter[1] | command_counter[0]==command_counter[2]);
	command = command_counter[0];
	precommand = command;
}
