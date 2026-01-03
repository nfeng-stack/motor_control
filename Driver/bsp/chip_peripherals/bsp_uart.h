#ifndef     __BSP_UART_H
#define     __BSP_UART_H
#include "stdint.h"
#include "stm32f1xx_hal.h"  
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_dma.h"
#define UART1_TX_PIN        GPIO_PIN_9
#define UART1_RX_PIN        GPIO_PIN_10
#define UART1_PIN_GROUP     GPIOA

#define UART3_TX_PIN        GPIO_PIN_10
#define UART3_RX_PIN        GPIO_PIN_11
#define UART3_PIN_GROUP     GPIOB

void bsp_uart3_init(void);
void bsp_uart1_init(void);
uint8_t usart1_read(uint8_t *is_success);
void uart1_send_byte(char *ch);


#endif