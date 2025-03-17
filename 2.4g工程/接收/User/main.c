#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "usart.h"
#include "led.h"
uint8_t Buf[DataWidth]={0};
uint16_t AD_x;
uint16_t AD_y;
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
		OLED_ShowString(1,1,"S:");
		OLED_ShowNum(1,3,state, 1);
		OLED_ShowString(2,1,"Suc");

		printf("state = %d \r\n CheckSuccess\r\n", state);
	}
	else if(state == CheckError)
	{
		OLED_ShowString(1,1,"State:");
		OLED_ShowNum(1,10,state, 1);
		OLED_ShowString(2,1,"CheckError");

		printf("state = %d \r\n CheckError\r\n", state);

	}
	OLED_ShowString(3,1,"Rece:");
	LED1_OFF();
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
		OLED_ShowHexNum(3,10,value,2);
        printf("value=%x\r\n", value);
        printf("\r\n");
#else if DataWidth == 5		
		if(R_IRQ()==0) 
		{
			Receive(Buf);
			OLED_ShowHexNum(3,6,Buf[0],2);
			OLED_ShowHexNum(3,9,Buf[1],2);
			OLED_ShowHexNum(3,12,Buf[2],2);
			OLED_ShowHexNum(4,6,Buf[3],2);
			OLED_ShowHexNum(4,9,Buf[4],2);
		}
			if(Buf[0] == 0x99)//摇杆ADC
			{
				
				AD_x = (Buf[1] << 8) | Buf[2];
				AD_y = (Buf[3] << 8) | Buf[4];
				OLED_ShowNum(1,70,AD_x, 4);
				OLED_ShowNum(2,70,AD_y, 4);

			}

			printf("Buf[0]=%x\r\n", Buf[0]);
			printf("Buf[1]=%x\r\n", Buf[1]);
			printf("Buf[2]=%x\r\n", Buf[2]);
			printf("Buf[3]=%x\r\n", Buf[3]);
			printf("Buf[4]=%x\r\n", Buf[4]);
			printf("\r\n");
#endif
	}
	
}
