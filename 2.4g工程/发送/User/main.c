#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "usart.h"
#include "NRF24L01_define.h"
#include "ad.h" 
#include "timer.h" 
#include "mykey.h" 
#include "myled.h" 
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
uint8_t keynum;
extern uint8_t Buf[DataWidth];
uint8_t  ROCK1_ADC_SendFlag, ROCK2_ADC_SendFlag;
uint16_t ROCK1_AD_X, ROCK1_AD_Y, ROCK2_AD_X, ROCK2_AD_Y;
uint8_t ROCK1_AD_X_HighByte, ROCK1_AD_X_LowByte, ROCK1_AD_Y_HighByte, ROCK1_AD_Y_LowByte, \
		ROCK2_AD_X_HighByte, ROCK2_AD_X_LowByte, ROCK2_AD_Y_HighByte, ROCK2_AD_Y_LowByte;
int16_t BAT_AD_VALUE;
int main(void)
{
    float pitch,roll,yaw; 		//欧拉角
    short aacx,aacy,aacz;		//加速度传感器原始数据
    short gyrox,gyroy,gyroz;	//陀螺仪原始数据
    short temp;					//温度	
	uint8_t state = 1;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	uart_init(115200);
    printf("Hello!\r\n");

	OLED_Init();
	My_Key_Init();
	My_LED_Init();
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
    MPU_Init();					//初始化MPU6050
    while(mpu_dmp_init())
    {
        printf("MPU6050 Error\r\n");
        Delay_ms(200);
    }  
	My_Buzzer_Init();
    BUZZER_On();
    Delay_ms(500);
    BUZZER_Off();
    TIM2_Int_Init(1000 -1, 72 -1);
	while (1)
	{
//    printf("Hello!\r\n");
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
/*ROCK1_KEY*/   
        if (keynum == ROCK1_KEY_SingleClick)
		{
            LEDx_SetMode(LED0, Mode_ON);   
			ROCK1_ADC_SendFlag = 1;
		}
        if (keynum == ROCK1_KEY_DobleClick)
		{
            LEDx_SetMode(LED0, Mode_SlowFlash);   
			ROCK1_ADC_SendFlag = 0;
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
        if (keynum == ROCK1_KEY_LongCick)
		{
            LEDx_SetMode(LED0, Mode_FastFlash);    
			ROCK1_ADC_SendFlag = 0;
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

/*ROCK2_KEY*/        
        if (keynum == ROCK2_KEY_SingleClick)
		{
            LEDx_SetMode(LED1, Mode_ON);   
			ROCK2_ADC_SendFlag = 1;
		}
        if (keynum == ROCK2_KEY_DobleClick)
		{

            LEDx_SetMode(LED1, Mode_SlowFlash);  
			ROCK2_ADC_SendFlag = 0;
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
        if (keynum == ROCK2_KEY_LongCick)
		{
            LEDx_SetMode(LED1, Mode_FastFlash);  
			ROCK2_ADC_SendFlag = 0;
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

/*KEY_L*/        
        if (keynum == KEY_L_SingleClick)
		{
            LEDx_SetMode(LED2, Mode_ON);   
			ROCK1_ADC_SendFlag = 1;
		}
        if (keynum == KEY_L_DobleClick)
		{
            LEDx_SetMode(LED2, Mode_SlowFlash);  
			ROCK1_ADC_SendFlag = 0;
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
        if (keynum == KEY_L_LongCick)
		{
            LEDx_SetMode(LED2, Mode_FastFlash);  
			ROCK1_ADC_SendFlag = 0;
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
/*KEY_R*/        
        if (keynum == KEY_R_SingleClick)
		{
                printf("aaaa\r\n");
            LEDx_SetMode(LED3, Mode_ON);   
			ROCK2_ADC_SendFlag = 1;
		}
        if (keynum == KEY_R_DobleClick)
		{
                printf("bbbb\r\n");
            LEDx_SetMode(LED3, Mode_SlowFlash);  
			ROCK2_ADC_SendFlag = 0;
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
        if (keynum == KEY_R_LongCick)
		{
                printf("cccc\r\n");
            LEDx_SetMode(LED3, Mode_FastFlash);  
			ROCK2_ADC_SendFlag = 0;
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
/*ROCK1*/
        ROCK1_AD_X = AD_GetValue(ROCK1_X_ADC_Channel);//ADC_X
		ROCK1_AD_Y = AD_GetValue(ROCK1_Y_ADC_Channel);//ADC_Y
		ROCK1_AD_X_HighByte = (ROCK1_AD_X >> 8) & 0xFF;//取出ADC_X的高8位
		ROCK1_AD_X_LowByte  =  ROCK1_AD_X & 0xFF;	   //取出ADC_X的低8位
		ROCK1_AD_Y_HighByte = (ROCK1_AD_Y >> 8) & 0xFF;//取出ADC_Y的高8位
		ROCK1_AD_Y_LowByte  =  ROCK1_AD_Y & 0xFF;	   //取出ADC_Y的低8位

//        OLED_ShowNum(1,92,ROCK1_AD_X, 4);//原始数据
//        OLED_ShowNum(2,92,ROCK1_AD_Y, 4);

//		OLED_ShowHexNum(1,86,ROCK1_AD_X_HighByte, 2);//移位数据
//	    OLED_ShowHexNum(1,88,ROCK1_AD_X_LowByte, 2);
//		OLED_ShowHexNum(2,86,ROCK1_AD_Y_HighByte, 2);
//	    OLED_ShowHexNum(2,88,ROCK1_AD_Y_LowByte, 2);

		if(ROCK1_ADC_SendFlag)				//发送摇杆ADC   
		{
			Buf[0] = 0x99;						//帧头
			Buf[1] = ROCK1_AD_X_HighByte;		//数据
			Buf[2] = ROCK1_AD_X_LowByte;		//数据
			Buf[3] = ROCK1_AD_Y_HighByte;		//数据			、
			Buf[4] = ROCK1_AD_Y_LowByte;		//数据	
            Send(Buf);
			
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}
/*ROCK2*/
        ROCK2_AD_X = AD_GetValue(ROCK2_X_ADC_Channel);//ADC_X
		ROCK2_AD_Y = AD_GetValue(ROCK2_Y_ADC_Channel);//ADC_Y
		ROCK2_AD_X_HighByte = (ROCK2_AD_X >> 8) & 0xFF;//取出ADC_X的高8位
		ROCK2_AD_X_LowByte  =  ROCK2_AD_X & 0xFF;	   //取出ADC_X的低8位
		ROCK2_AD_Y_HighByte = (ROCK2_AD_Y >> 8) & 0xFF;//取出ADC_Y的高8位
		ROCK2_AD_Y_LowByte  =  ROCK2_AD_Y & 0xFF;	   //取出ADC_Y的低8位

        OLED_ShowNum(1,92,ROCK2_AD_X, 4);//原始数据
        OLED_ShowNum(2,92,ROCK2_AD_Y, 4);

		OLED_ShowHexNum(1,86,ROCK2_AD_X_HighByte, 2);//移位数据
	    OLED_ShowHexNum(1,88,ROCK2_AD_X_LowByte, 2);
		OLED_ShowHexNum(2,86,ROCK2_AD_Y_HighByte, 2);
	    OLED_ShowHexNum(2,88,ROCK2_AD_Y_LowByte, 2);

		if(ROCK2_ADC_SendFlag)				//发送摇杆ADC   
		{
			Buf[0] = 0x99;						//帧头
			Buf[1] = ROCK2_AD_X_HighByte;		//数据
			Buf[2] = ROCK2_AD_X_LowByte;		//数据
			Buf[3] = ROCK2_AD_Y_HighByte;		//数据			、
			Buf[4] = ROCK2_AD_Y_LowByte;		//数据	
            Send(Buf);
			
			OLED_ShowHexNum(3,6,Buf[0], 2);
			OLED_ShowHexNum(3,9,Buf[1], 2);
			OLED_ShowHexNum(3,12,Buf[2], 2);
			OLED_ShowHexNum(4,6,Buf[3], 2);
			OLED_ShowHexNum(4,9,Buf[4], 2);
		}	
/*BAT*/
		/*
			VBAT/50K = BAT/(50K+50K)
			VBAT = BAT*0.5
			BAT = VBAT * 2
		*/
//		BAT_AD_VALUE = (AD_GetValue(BAT_ADC_Channel)) * 2;
//		printf("BAT_AD_VALUE：%d\r\n", BAT_AD_VALUE);
/*MPU6050*/
        if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
        { 
            temp=MPU_Get_Temperature();	//得到温度值
            printf("\n\r pitch=%.2f      roll=%.2f      yaw=%.2f      temp=%2d.%d\n\r",pitch,roll,yaw, temp/100, temp%10); 
            MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
            MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
        }
	}
}
