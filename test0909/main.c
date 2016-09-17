#include"stm32f10x.h"
#include"initialsetting.h"
#include"initialsetting_controller.h"
#include"delay.h"
#include"servo.h"
#include"motion.h"

void GPIO_Configuration(void);
void Conversepacket();
void Conversepacket_torque();
//void Conversepacket(servo servo);
//void Conversepacket_torque(servo servo);

int main(void)
{
	SystemInit();
	GPIO_Configuration();
	USART1_Configuration();
	delay_Configuration();
	while(1){
		motion_select();
	}
}
