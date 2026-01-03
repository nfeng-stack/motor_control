#include "bsp_uart.h"

extern uint8_t rx_buff[64];

/**
 * 标准输入输出口，不向上提供能力
 */
void bsp_uart3_init(void)
{
    UART_HandleTypeDef huart3;
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.Mode = USART_MODE_TX_RX;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    HAL_UART_Init(&huart3);

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pin = UART3_TX_PIN ;/*TX*/
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(UART3_PIN_GROUP, &GPIO_InitStructure);
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pin = UART3_RX_PIN;
    HAL_GPIO_Init(UART3_PIN_GROUP, &GPIO_InitStructure);

    __HAL_RCC_DMA1_CLK_ENABLE();
    DMA_HandleTypeDef hdma;
    hdma.Instance = DMA1_Channel3;
    hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma.Init.MemInc = DMA_MINC_ENABLE;
    hdma.Init.Mode = DMA_NORMAL;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma);

    HAL_NVIC_SetPriority(USART3_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    LL_USART_EnableDMAReq_RX(USART3);
    LL_USART_EnableIT_IDLE(USART3);

    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)&(USART3->DR));
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)rx_buff);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, sizeof(rx_buff));

    LL_USART_Enable(USART3);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}



void bsp_uart1_init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	GPIO_InitTypeDef  GPIO_Initstructure;
	GPIO_Initstructure.Mode = GPIO_MODE_AF_PP;
	GPIO_Initstructure.Pin = UART1_TX_PIN;
	GPIO_Initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Initstructure.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(UART1_PIN_GROUP, &GPIO_Initstructure);
	
	GPIO_Initstructure.Mode = GPIO_MODE_AF_INPUT;
	GPIO_Initstructure.Pin = UART1_RX_PIN;
	GPIO_Initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Initstructure.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(UART1_PIN_GROUP, &GPIO_Initstructure);
	
	UART_HandleTypeDef huart1 = {0};
	huart1.Instance = USART1 ;
	huart1.Init.BaudRate = 9600 ;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE ;
	huart1.Init.Mode = UART_MODE_TX_RX ;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16 ;
	huart1.Init.Parity = UART_PARITY_NONE ;
	huart1.Init.StopBits = UART_STOPBITS_1 ;
	huart1.Init.WordLength = UART_WORDLENGTH_8B ;
	HAL_UART_Init(&huart1);
	__HAL_USART_ENABLE_IT(&huart1,USART_IT_RXNE);	
	
	HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
	__HAL_USART_ENABLE(&huart1);
}
typedef struct
{
    uint8_t buffer[16];
    uint8_t write_position;
    uint8_t read_position;
}ring_buffer;
ring_buffer usart1_rx_buffer = {0};

void usart1_write(uint8_t data)
{
    uint8_t len = usart1_rx_buffer.write_position - usart1_rx_buffer.read_position;
    if (len < 16) {
        usart1_rx_buffer.buffer[usart1_rx_buffer.write_position++ & 0x0f] = data;
    } else {
        return ;
    }
}

uint8_t usart1_read(uint8_t *is_success)
{
    if (usart1_rx_buffer.write_position == usart1_rx_buffer.read_position) {
        *is_success = 0;
        return 0;
    } else {
        *is_success = 1;
        return usart1_rx_buffer.buffer[usart1_rx_buffer.read_position++ & 0x0f];
    }
}

void USART1_IRQHandler(void)
{
	if(LL_USART_IsActiveFlag_RXNE(USART1))
	{
        usart1_write(LL_USART_ReceiveData8(USART1));
		LL_USART_ClearFlag_RXNE(USART1);
	}
}

void uart1_send_byte(char *ch)
{
    if(ch == NULL) return ;
    if (!LL_USART_IsEnabled(USART1)) {
        LL_USART_Enable(USART1);
    }
    while(*ch != '\0') {
        LL_USART_TransmitData8(USART1,*ch++);
        while(!LL_USART_IsActiveFlag_TXE(USART1));
    }
    while(!LL_USART_IsActiveFlag_TC(USART1));
}