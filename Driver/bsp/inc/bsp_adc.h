#ifndef     __BSP_ADC_H
#define     __BSP_ADC_H

extern uint16_t adc_getvalue(void);
extern void adc_init(void);
extern void adc_st_dma(uint8_t flag);
extern uint16_t adc_buff[36];
extern uint16_t adc_buff2[36];
extern uint8_t adc_flag;
#endif