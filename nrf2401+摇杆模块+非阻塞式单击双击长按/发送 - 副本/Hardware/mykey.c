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
    �˴��벻��������жϷ�����Temp = Key_Num; Key_Num = 0; ������֮�䣬
    �����ж������˼����־λ����ô�ж��˳��󣬻�����ִ��key_Nm=0��
    �����ٴε��ô˺������Ƿ���0�ˣ�Ҳ���Ǵ˴ΰ����¼��ͻᱻ���ԡ�
    ����Ҫ���һ��if�жϣ����key_num��Ϊ0����ִ�ж�����0�Ĳ��� 
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

uint8_t Key_GetState(void)//��ȡ����״̬
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)//����1����
    {
        return 1;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)//����2����
    {
        return 2;
    }
    return 0;//�ް�������
}


void Key_Tick(void)//����ɨ��1ms����һ��
{
    //��̬����Ĭ�ϳ�ֵΪ0
    static uint8_t Count;                           //����ֵ
    static uint8_t CurrentKeyVal, PreviousKeyVal;           //����״̬����һ��״̬
    static uint16_t PressDownCnt;
    static uint16_t RePressDownCnt;
    static uint16_t PressUpCnt;
    static uint8_t step;  
    static uint8_t key1flag, key2flag;
    Count ++;
    if (Count >= 1)                                //20ms����һ��Key_GetState
    {
        Count = 0;

        PreviousKeyVal = CurrentKeyVal;                     //�ϴ�״̬��Ҳ������һ�εı���
        CurrentKeyVal = Key_GetState();                //����״̬
    }
    if(step == 0)
    {
        PressDownCnt = 0; 
        RePressDownCnt = 0;
        PressUpCnt = 0;
        if (CurrentKeyVal !=0  && PreviousKeyVal == 0)      //����Ϊ0 �ϴη�0��˵���а������²�����  ����Ϊ1 �ɿ�Ϊ0
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
        

