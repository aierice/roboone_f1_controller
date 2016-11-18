#include"stm32f10x.h"
#include"initialsetting.h"
#include"initialsetting_controller.h"
#include"delay.h"
#include"tim1.h"
#include"servo.h"
#include"motion.h"

uint8_t precommand;
uint8_t command;

//void GPIO_Configuration(void);
//void Conversepacket();
//void Conversepacket_torque();
//void Conversepacket(servo servo);
//void Conversepacket_torque(servo servo);

int main(void)
{
	SystemInit();
	GPIO_Configuration();
	USART1_Configuration();
	delay_Configuration();
	precommand = 0b00000000;
	uint8_t i;
	while(1){
		//GPIOC:LED high>LED off.low>LED on.
		//GPIOA:switch neutral>high push>low
		//neutral>switch>high>LED>off
//		USART_SendData(USART1,(uint8_t)(((uint8_t)~GPIO_ReadInputData(GPIOA) >>4)&0b10000000));
//		tdelay(2000);
//		USART_SendData(USART1,(uint8_t)((uint8_t)~GPIO_ReadInputData(GPIOA)&0b00001111));
//		tdelay(1000);
		USART_SendData(USART1,(~GPIO_ReadInputData(GPIOA) >>4)|0b0000000010000000);
		tdelay(5);
		USART_SendData(USART1,~GPIO_ReadInputData(GPIOA)&0b0000000000001111);
		tdelay(95);
//		USART_SendData(USART1,(uint8_t)GPIO_ReadInputData(GPIOA));
		GPIO_Write(GPIOC,GPIO_ReadInputData(GPIOA)>>4);
//		motion_select();
/*		for(i=0; i<=3; i++){
			switch(i){
			case 0:	GPIO_SetBits(GPIOC,GPIO_Pin_0);
					break;
			case 1:	GPIO_SetBits(GPIOC,GPIO_Pin_1);
					break;
			case 2:	GPIO_SetBits(GPIOC,GPIO_Pin_2);
					break;
			case 3:	GPIO_SetBits(GPIOC,GPIO_Pin_3);
					break;
			default: break;

			case 0:	GPIO_Write(GPIOC,~0b00000001);
					break;
			case 1:	GPIO_Write(GPIOC,~0b00000010);
					break;
			case 2:	GPIO_Write(GPIOC,~0b00000100);
					break;
			case 3:	GPIO_Write(GPIOC,~0b00001000);
					break;
			default: break;
			}
			tdelay(1000);
		}
*/
	}
}
