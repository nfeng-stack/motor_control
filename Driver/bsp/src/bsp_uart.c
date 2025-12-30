#include "stm32f1xx_hal.h"
#include "bsp_init.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_dma.h"
#include "stdio.h"
#include "string.h"
 extern uint8_t rx_buff[64];

void uart_init(void)
{
    UART_HandleTypeDef huart1;
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.Mode = USART_MODE_TX_RX;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    HAL_UART_Init(&huart1);

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pin = GPIO_PIN_9 ;/*TX*/
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    __HAL_RCC_DMA1_CLK_ENABLE();
    DMA_HandleTypeDef hdma;
    hdma.Instance = DMA1_Channel5;
    hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma.Init.MemInc = DMA_MINC_ENABLE;
    hdma.Init.Mode = DMA_NORMAL;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma);

    HAL_NVIC_SetPriority(USART1_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    LL_USART_EnableDMAReq_RX(USART1);
    LL_USART_EnableIT_IDLE(USART1);

    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)&(USART1->DR));
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)rx_buff);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, sizeof(rx_buff));

    LL_USART_Enable(USART1);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}