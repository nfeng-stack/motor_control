#include "stm32f1xx_hal.h"  
#include "stm32f1xx_ll_usart.h"                // Device header
#include "OLED.h"
#include "rtthread.h"

uint8_t receivedata;
uint8_t flagdata;

void Send_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	GPIO_InitTypeDef  GPIO_Initstructure;
	GPIO_Initstructure.Mode = GPIO_MODE_AF_PP;
	GPIO_Initstructure.Pin = GPIO_PIN_9;
	GPIO_Initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Initstructure.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	GPIO_Initstructure.Mode = GPIO_MODE_AF_INPUT;
	GPIO_Initstructure.Pin = GPIO_PIN_10;
	GPIO_Initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Initstructure.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(GPIOA, &GPIO_Initstructure);
	
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
/*
void Send_Byte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Send_Arry(uint8_t *Arry,uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		Send_Byte(Arry[i]);
	}
}

void Send_String(char *string)
{
	uint8_t i;
	for(i = 0; string[i] != 0; i++)
	{
		Send_Byte(string[i]);
	}
}

uint32_t send_pow(uint32_t x,uint32_t y)
{
	unsigned int Result = 1;
	while(y--)
	{
		Result *= x;
	}
	return Result;
}

void send_number(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length; i++)
	{
		Send_Byte(Number / send_pow(10,Length - i - 1) % 10 + '0');
	}
}
*/
uint8_t save_flagdata(void)
{
	if(flagdata == 1)
	{
		flagdata = 0;
		return 1;
	}
	return 0;
}

uint8_t GET_receivedata(void)
{
	return receivedata;
}

void USART1_IRQHandler()
{
	if(LL_USART_IsActiveFlag_RXNE(USART1))
	{
		receivedata = LL_USART_ReceiveData8(USART1);
		flagdata = 1;
		OLED_ShowHexNum(2,1,receivedata,3);
		// if(save_flagdata() == 1)
		// {
		// 	data = GET_receivedata();
		// 	OLED_ShowHexNum(2,1,data,2);
		// }
		LL_USART_ClearFlag_RXNE(USART1);
	}
}


