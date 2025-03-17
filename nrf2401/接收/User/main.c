#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "usart.h"
#include "led.h"
	uint8_t Buf[DataWidth]={0};
int main(void)
{

    uint8_t value;
	uint8_t state = 1;

	uart_init(115200);
    printf("hello\r\n");
	OLED_Init();
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

#if DataWidth == 1
		if(R_IRQ()==0) 
		{
            
			value = ReceiveOneChar();
		}
		 
		if(value == 0x80)
		{
			LED1_ON();

		}
		else if(value == 0x90)
		{
			LED1_OFF();
		}
        printf("value=%x\r\n", value);
        printf("\r\n");
#else if DataWidth == 5		
		if(R_IRQ()==0) 
		{
			Receive(Buf);
		}
		OLED_ShowHexNum(1,1,Buf[0],2);
		OLED_ShowHexNum(1,4,Buf[1],2);
		OLED_ShowHexNum(2,1,Buf[2],2);
		OLED_ShowHexNum(2,4,Buf[3],2);
		OLED_ShowHexNum(3,1,Buf[4],2);

		printf("Buf[0]=%x\r\n", Buf[0]);
		printf("Buf[1]=%x\r\n", Buf[1]);
		printf("Buf[2]=%x\r\n", Buf[2]);
		printf("Buf[3]=%x\r\n", Buf[3]);
		printf("Buf[4]=%x\r\n", Buf[4]);
		printf("\r\n");
#endif
	}
}
