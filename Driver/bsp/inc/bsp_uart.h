#ifndef     __BSP_UART_H
#define      __BSP_UART_H

extern void uart_init(void);
extern void uart_tx(uint8_t *data, uint8_t length);

#endif