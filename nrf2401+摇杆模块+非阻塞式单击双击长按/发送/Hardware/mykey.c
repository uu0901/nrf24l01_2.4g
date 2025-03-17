#include "stm32f10x.h"                  // Device header
#include "mykey.h"                  // Device header
uint8_t Key_Num;

void My_Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    /* 
    此代码不合理，如果中断发生在Temp = Key_Num; Key_Num = 0; 这两句之间，
    并且中断里置了键码标志位，那么中断退出后，回立刻执行key_Nm=0，
    这样再次调用此函数就是返回0了，也就是此次按键事件就会被忽略。
    所以要多加一个if判断，如果key_num不为0，再执行读后清0的操作 
    */
    //uint8_t Temp;
    //Temp = Key_Num;
    //Key_Num = 0;
    //return Temp;
    

    uint8_t Temp;
    if (Key_Num)
    {
        Temp = Key_Num;
        Key_Num = 0;

        return Temp;
    }
    return 0;
}

uint8_t Key_GetState(void)//获取按键状态
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)//按键1按下
    {
        return 1;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)//按键2按下
    {
        return 2;
    }
    return 0;//无按键按下
}


void Key_Tick(void)//按键扫描1ms调用一次
{
    //静态变量默认初值为0
    static uint8_t Count;                           //计数值
    static uint8_t CurrentKeyVal, PreviousKeyVal;           //本次状态和上一次状态
    static uint16_t PressDownCnt;
    static uint16_t RePressDownCnt;
    static uint16_t PressUpCnt;
    static uint8_t step;  
    static uint8_t key1flag, key2flag;
    Count ++;
    if (Count >= 1)                                //20ms调用一次Key_GetState
    {
        Count = 0;

        PreviousKeyVal = CurrentKeyVal;                     //上次状态，也就是上一次的本次
        CurrentKeyVal = Key_GetState();                //本次状态
    }
    if(step == 0)
    {
        PressDownCnt = 0; 
        RePressDownCnt = 0;
        PressUpCnt = 0;
        if (CurrentKeyVal !=0  && PreviousKeyVal == 0)      //本次为0 上次非0，说明有按键按下并松手  按下为1 松开为0
        {
            step = 1;
            if(CurrentKeyVal == 1) key1flag = 1;
            if(CurrentKeyVal == 2) key2flag = 1;
        }
    }
    else if (step == 1)
    {
        if(CurrentKeyVal != 0 )
        {
            PressDownCnt ++;
            if(PressDownCnt >= LongTime)
            {
                if(CurrentKeyVal == 1)
                {
                    Key_Num = Key1LongCick;
                    key1flag = 0;
                }
                else if(CurrentKeyVal == 2)
                {
                    Key_Num = Key2LongCick;
                      key2flag = 0;
                }

                step = 0;
            }
        }
        else 
        {

            step = 2;
        }
    }  
    else if (step == 2)
    {
        PressUpCnt ++;
        if(PressUpCnt <= DoubleTime)
        {
            if(CurrentKeyVal != 0)
            {
                RePressDownCnt ++;
            }
            if(RePressDownCnt > KeyDelay)
            {
                if(CurrentKeyVal == 1)
                {
                    key1flag = 0;
                    Key_Num = Key1DobleClick;
                }
                 else if(CurrentKeyVal == 2)
                {
                    key2flag = 0;
                    Key_Num = Key2DobleClick;
                }
                step = 0;
            }
        }
        else
        {
 
            if(PressDownCnt > KeyDelay)
            {
                if(key1flag == 1)
                {
                    key1flag = 0;
                    Key_Num = Key1SingleClick;
                }
                else if(key2flag == 1)
                {
                    key2flag = 0;
                    Key_Num = Key2SingleClick;
                }
                step = 0;
            }
        }
    }
}
        

