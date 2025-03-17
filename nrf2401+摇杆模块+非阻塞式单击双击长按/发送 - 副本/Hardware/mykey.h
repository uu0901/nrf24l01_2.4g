#ifndef __MYKEY_H
#define __MYKEY_H
#define LongTime        1500
#define DoubleTime      500
#define KeyDelay        30

#define Key1SingleClick     1
#define Key1DobleClick      2
#define Key1LongCick        3
#define Key2SingleClick     4
#define Key2DobleClick      5
#define Key2LongCick        6
void My_Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);

#endif
