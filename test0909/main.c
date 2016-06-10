#include"stm32f10x.h"

typedef struct{
	uint8_t flug;
	uint8_t ID;
	uint8_t on_off;
	int16_t degree;	//�������10�{�͎蓮��
	int16_t time;		//5000ms = 5s�B5000�Ə���
	uint16_t pdegree;	//���ݒn
} servo;

uint32_t timingdelay;

void GPIO_Configuration(void);
void USART1_Configuration(void);
void Conversepacket(servo servo);
void Conversepacket_torque(servo servo);
void tdelay(uint32_t nTime);


uint8_t sendbuf[200];
uint8_t checksum;

int main(void)
{
	SystemInit();
	GPIO_Configuration();
	USART1_Configuration();
	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}
	servo servo[30];
	uint8_t i;
	for(i = 1; i<=1; i++){
		servo[i].flug = 1;
		servo[i].ID = 1;
		servo[i].on_off = 1;
		servo[i].degree = -1500;
		servo[i].time = 0;
		Conversepacket_torque(servo[i]);
		Conversepacket(servo[i]);
	}
	while(1)
    {
		while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
		for(i = 1; i<=1; i++){
			if(servo[i].degree<1500){
				servo[i].degree += 300;
			}
			else servo[i].degree = -1500;
			Conversepacket(servo[i]);
		}
		tdelay(1000);
    }
}

void GPIO_Configuration(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void USART1_Configuration(void){
	//USART1��RX�s����PA10�B����clock�͗^���Ă���
	//USART1��TX�s����PA9

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);

	USART_Cmd(USART1,ENABLE);
}

void Conversepacket(servo servo){
	memset( sendbuf, 0x00, sizeof( sendbuf ));
	if(servo.flug!=1) return;
	sendbuf[0] = ( uint8_t) 0xFA;		// �w�b�_�[1
	sendbuf[1] = ( uint8_t) 0xAF;		// �w�b�_�[2
	sendbuf[2] = servo.ID;			// �T�[�{ID
	sendbuf[3] = ( uint8_t) 0x00;		// �t���O //0x00�Ń��^�[���p�P�b�g�����B�������^�[���p�P�b�g���~�����Ȃ�0x0F
	sendbuf[4] = ( uint8_t) 0x1E;		// �A�h���X //0x1E=30�B�Œ�l�B���^�[���p�P�b�g�A���Ɍ��ݍ��W���~�����Ȃ��0x2A�B�d��(����)���~�����Ȃ��0x30
	sendbuf[5] = ( uint8_t) 0x04;		//�f�[�^�̒����B����(2)+����(2)�B�����O�p�P�b�g�̏ꍇ�A������VID(1)���������B���^�[���p�P�b�g�A���Ɍ��ݍ��W�݂̂��~�����Ȃ��0x02
	sendbuf[6] = ( uint8_t) 0x01;		// ��
	sendbuf[7] = (( uint16_t) servo.degree) &0xFF;
	sendbuf[8] = (( uint16_t) servo.degree) >>8;
	sendbuf[9] = (( uint16_t) servo.time) &0xFF;
	sendbuf[10]= (( uint16_t) servo.time) >>8;	//�����ƃX�}�[�g�ɏ�������
	checksum = 0;
	uint8_t i;
	for( i = 2; i <=10; i++){
		checksum = checksum ^ sendbuf[i];
	}
	sendbuf[11] = checksum;
	for( i = 0; i <= 11; i++){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	USART_SendData(USART1,sendbuf[i]);
	}
	return;
}

void Conversepacket_torque(servo servo){
	memset( sendbuf, 0x00, sizeof( sendbuf ));
	if(servo.flug!=1) return;
	sendbuf[0] = ( uint8_t) 0xFA;		// �w�b�_�[1
	sendbuf[1] = ( uint8_t) 0xAF;		// �w�b�_�[2
	sendbuf[2] = servo.ID;			// �T�[�{ID
	sendbuf[3] = ( uint8_t) 0x00;		// �t���O //0x00�Ń��^�[���p�P�b�g�����B�������^�[���p�P�b�g���~�����Ȃ�0x0F
	sendbuf[4] = ( uint8_t) 0x24;		// �A�h���X //0x1E=30�B�Œ�l�B���^�[���p�P�b�g�A���Ɍ��ݍ��W���~�����Ȃ��0x2A�B�d��(����)���~�����Ȃ��0x30�B�g���N�I���Ȃ�0x24
	sendbuf[5] = ( uint8_t) 0x01;		//�f�[�^�̒����B����(2)+����(2)�B�����O�p�P�b�g�̏ꍇ�A������VID(1)���������B���^�[���p�P�b�g�A���Ɍ��ݍ��W�݂̂��~�����Ȃ��0x02�B�g���N�I���Ȃ�0x01
	sendbuf[6] = ( uint8_t) 0x01;		// ��
	sendbuf[7] = ( uint8_t) 0x01;
	checksum = 0;
	uint8_t i;
	for( i = 2; i <=7; i++){
		checksum = checksum ^ sendbuf[i];
	}
	sendbuf[8] = checksum;
	for( i = 0; i <= 8; i++){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	USART_SendData(USART1,sendbuf[i]);
	}
	return;
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

