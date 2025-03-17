#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "usart.h"
#include "key.h"
#include "led.h"
#include "NRF24L01_define.h"
uint8_t keynum;
extern uint8_t Buf[DataWidth];
extern uint8_t Buf1[DataWidth];
int main(void)
{
	uint8_t state = 1;
	uart_init(115200);
    printf("Hello!\r\n");

	OLED_Init();

	Key_Init();
	LED_Init();

	state = NRF24L01_Init();
	if( state == CheckSuccess)
	{
		OLED_ShowString(1,1,"State:");
		OLED_ShowNum(1,10,state, 1);
		OLED_ShowString(2,1,"CheckSuccess");

		printf("state = %d \r\n CheckSuccess\r\n", state);
	}
	else if(state == CheckError)
	{
		OLED_ShowString(1,1,"State:");
		OLED_ShowNum(1,10,state, 1);
		OLED_ShowString(2,1,"CheckError");

		printf("state = %d \r\n CheckError\r\n", state);

	}

	while (1)
	{

        keynum = Key_GetNum();
#if DataWidth == 1
		if(keynum == 1)
		{
			SendOneChar(0x80);
			OLED_ShowString(3,1,"Send:");
			OLED_ShowHexNum(3,10,0x80, 2);
		}
		if (keynum == 2)
		{
			SendOneChar(0x90);
			OLED_ShowString(3,1,"Send:");
			OLED_ShowHexNum(3,10,0x90, 2);
		}
#else if DataWidth == 5
		if(keynum == 1)
		{
			Send(Buf);
			OLED_ShowString(3,1,"Send:");
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}
		if (keynum == 2)
		{
			Send(Buf1);
			OLED_ShowString(3,1,"Send:");
			OLED_ShowHexNum(3,6,Buf1[0], 2);
			OLED_ShowHexNum(3,9,Buf1[1], 2);
			OLED_ShowHexNum(3,12,Buf1[2], 2);
			OLED_ShowHexNum(4,6,Buf1[3], 2);
			OLED_ShowHexNum(4,9,Buf1[4], 2);
		}
#endif
	}
}
