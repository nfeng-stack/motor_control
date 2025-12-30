#ifndef __USART_H
#define __USART_H

void Send_Init(void);
//void Send_Byte(uint8_t Byte);
//void Send_Arry(uint8_t *Arry,uint16_t Length);
//void send_number(uint32_t Number, uint8_t Length);
uint8_t save_flagdata(void);
uint8_t GET_receivedata(void);

#endif
