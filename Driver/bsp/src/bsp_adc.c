#include "stm32f1xx_hal.h"
#include "bsp_adc.h"
/**
 * @brief   ADC1的通道的通道12，对应的端口为PC2
 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc;
uint16_t adc_buff[36];
uint16_t adc_buff2[36];
void adc_init(void)
{
    hadc1.Instance = ADC1;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;/*定时器三触发，只能上升沿触发*/
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.ScanConvMode = DISABLE;
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    HAL_ADC_Init(&hadc1);
   
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_ChannelConfTypeDef sconfig = {0};
    sconfig.Channel = ADC_CHANNEL_12;
    sconfig.Rank = ADC_REGULAR_RANK_1;
    sconfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    HAL_ADC_ConfigChannel(&hadc1, &sconfig);

    ADC_MultiModeTypeDef  adc_mode_cfg ;
    adc_mode_cfg.Mode = ADC_MODE_INDEPENDENT ;
    HAL_ADCEx_MultiModeConfigChannel(&hadc1,&adc_mode_cfg);
    __HAL_RCC_DMA1_CLK_ENABLE();
    hdma_adc.Instance = DMA1_Channel1 ;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY ;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD ;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE ;
    hdma_adc.Init.Mode = DMA_NORMAL ;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD ;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE ;
    hdma_adc.Init.Priority = DMA_PRIORITY_LOW ;
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn,2,1);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_DMA_Init(&hdma_adc);
    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc);
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_buff,sizeof(adc_buff)/sizeof(uint16_t));
}

uint16_t adc_getvalue(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); /*只有在一个序列转换完成后才有完成标志位*/
    return HAL_ADC_GetValue(&hadc1);
}
void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc);
}
void adc_st_dma(uint8_t flag)
{
    if(flag == 1)
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_buff2,sizeof(adc_buff2)/sizeof(uint16_t));
    if(flag == 2)  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_buff,sizeof(adc_buff)/sizeof(uint16_t));
 
}
uint8_t adc_flag ;
extern uint16_t data;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  switch (adc_flag)
  {
  case 1:
    adc_flag = 2;
    data = 1;
    break;
  case 2:
    adc_flag = 1;
    data = 1;
    break;
  default:
    data = 1;
    adc_flag = 1;
    break;
  }
}