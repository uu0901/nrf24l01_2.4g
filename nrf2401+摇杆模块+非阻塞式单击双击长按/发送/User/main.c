#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "usart.h"
//#include "key.h"
//#include "led.h"
#include "NRF24L01_define.h"
#include "ad.h" 
#include "timer.h" 
#include "mykey.h" 
#include "myled.h" 
uint8_t keynum;
extern uint8_t Buf[DataWidth];
extern uint8_t Buf1[DataWidth];
uint16_t AD_x,AD_y;
uint8_t ADC_SendFlag;
uint8_t AD_x_HighByte;
uint8_t AD_x_LowByte;
uint8_t AD_y_HighByte;
uint8_t AD_y_LowByte;
int main(void)
{
	uint8_t state = 1;
	uart_init(115200);
    printf("Hello!\r\n");

	OLED_Init();

	My_Key_Init();
	My_LED_Init();
    TIM2_Int_Init(1000 -1, 72 -1);
	AD_Init();
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
		OLED_ShowNum(1,12,state, 1);
		OLED_ShowString(2,1,"Error");

		printf("state = %d \r\n CheckError\r\n", state);

	}
	OLED_ShowString(3,1,"Send:");
	while (1)
	{

        keynum = Key_GetNum();
#if DataWidth == 1
		if(keynum == Key2SingleClick)
		{
			SendOneChar(0x80);

			OLED_ShowHexNum(3,10,0x80, 2);
		}
		if (keynum == Key2DobleClick)
		{
			SendOneChar(0x90);

			OLED_ShowHexNum(3,10,0x90, 2);
		}
		if (keynum == Key2LongCick)
		{
			SendOneChar(0xAA);

			OLED_ShowHexNum(3,10,0xAA, 2);
		}
#else if DataWidth == 5
//		if(keynum == 1)
//		{
//			Send(Buf);
//			OLED_ShowString(3,1,"Send:");
//			OLED_ShowHexNum(3,6,Buf[0], 2);
//			OLED_ShowHexNum(3,9,Buf[1], 2);
//			OLED_ShowHexNum(3,12,Buf[2], 2);
//			OLED_ShowHexNum(4,6,Buf[3], 2);
//			OLED_ShowHexNum(4,9,Buf[4], 2);
//		}

        if (keynum == Key2SingleClick)
		{
            LED1_SetMode(1);    //����LED1ģʽ
			ADC_SendFlag = 1;
		}
        if (keynum == Key2DobleClick)
		{
            LED1_SetMode(2);    //����LED1ģʽ
			ADC_SendFlag = 0;
			Buf[0] = 0x99;		//帧头
			Buf[1] = 0x02;		//数据
			Buf[2] = 0;		//数据
			Buf[3] = 0;		//数据			、
			Buf[4] = 0;		//数据	
			Send(Buf);	
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}
        if (keynum == Key2LongCick)
		{
            LED1_SetMode(3);    //����LED1ģʽ
			ADC_SendFlag = 0;
			Buf[0] = 0x99;		//帧头
			Buf[1] = 0x03;		//数据
			Buf[2] = 0;		//数据
			Buf[3] = 0;		//数据			、
			Buf[4] = 0;		//数据		
            Send(Buf);
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}

#endif
        
        if (keynum == Key1SingleClick)
		{

            LED1_SetMode(1);    //����LED1ģʽ
			ADC_SendFlag = 1;
		}
        if (keynum == Key1DobleClick)
		{

            LED1_SetMode(2);    //����LED1ģʽ
			ADC_SendFlag = 0;
			Buf[0] = 0x99;		//帧头
			Buf[1] = 0x02;		//数据
			Buf[2] = 0;		//数据
			Buf[3] = 0;		//数据			、
			Buf[4] = 0;		//数据	
			Send(Buf);	
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}
        if (keynum == Key1LongCick)
		{

            LED1_SetMode(3);    //����LED1ģʽ
			ADC_SendFlag = 0;
			Buf[0] = 0x99;		//帧头
			Buf[1] = 0x03;		//数据
			Buf[2] = 0;		//数据
			Buf[3] = 0;		//数据			、
			Buf[4] = 0;		//数据		
            Send(Buf);
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}


        AD_x = AD_GetValue(ADC_Channel_0);//ADC_X
		AD_y = AD_GetValue(ADC_Channel_1);//ADC_Y
		AD_x_HighByte = (AD_x >> 8) & 0xFF;//取出ADC_X的高8位
		AD_x_LowByte  =  AD_x & 0xFF;	   //取出ADC_X的低8位
		AD_y_HighByte = (AD_y >> 8) & 0xFF;//取出ADC_Y的高8位
		AD_y_LowByte  =  AD_y & 0xFF;	   //取出ADC_Y的低8位

        OLED_ShowNum(1,92,AD_x, 4);
        OLED_ShowNum(2,92,AD_y, 4);

		OLED_ShowHexNum(1,86,AD_x_HighByte, 2);
	    OLED_ShowHexNum(1,88,AD_x_LowByte, 2);
		OLED_ShowHexNum(2,86,AD_y_HighByte, 2);
	    OLED_ShowHexNum(2,88,AD_y_LowByte, 2);

		if(ADC_SendFlag)				//发送摇杆ADC
		{
			Buf[0] = 0x99;				//帧头
			Buf[1] = AD_x_HighByte;		//数据
			Buf[2] = AD_x_LowByte;		//数据
			Buf[3] = AD_y_HighByte;		//数据			、
			Buf[4] = AD_y_LowByte;		//数据	
            Send(Buf);
			
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}
	}
}
