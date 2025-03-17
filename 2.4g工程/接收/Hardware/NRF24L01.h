#ifndef _NRF24L01_H
#define _NRF24L01_H


#define DataWidth           5
#define TxAddrWidth         5
#define RxAddrWidth         5

#define CheckSuccess    0
#define CheckError      1 
void W_Reg(uint8_t Reg,uint8_t Value);
uint8_t R_Reg(uint8_t Reg);
void W_Buf(uint8_t Reg , uint8_t* Buf, uint8_t Len);
void R_Buf(uint8_t Reg , uint8_t* Buf, uint8_t Len);
uint8_t NRF24L01_Init(void);
void Receive(uint8_t* Buf);
uint8_t Send(uint8_t* Buf);
uint8_t SendOneChar(uint8_t value);
uint8_t ReceiveOneChar(void);
uint8_t NRF24L01_Check(void);

uint8_t NRF24L01_Check_one(void);
#endif









