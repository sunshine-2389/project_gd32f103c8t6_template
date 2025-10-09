#ifndef _adc_read_H
#define _adc_read_H

// adc_read.h
// This header file defines the ADC reading functionality for the GD32F10x series microcontroller
#include "gd32f10x.h"

/*ADC gpio setting*/
#define ADC_SW              GPIO_PIN_11
#define ADC_2_7             GPIO_PIN_1
#define ADC_5_4             GPIO_PIN_2
#define ADC_SW_GPIO_PORT    GPIOA

#define ADC_27_PORT 			GPIOA  
#define ADC_27_PIN 				GPIO_PIN_1
#define ADC_27_PORT_RCC			RCU_GPIOA

#define ADC_54_PORT 			GPIOA  
#define ADC_54_PIN 				GPIO_PIN_2
#define ADC_54_PORT_RCC			RCU_GPIOA

#define ADC_SW_PORT 			GPIOA
#define ADC_SW_PIN 				GPIO_PIN_11
#define ADC_SW_PORT_RCC			RCU_GPIOA

#define ADC_CHANNEL_NUM 2 // Number of ADC channels to read

#define ADC_SET_SW gpio_bit_set(ADC_SW_PORT, ADC_SW_PIN)
#define ADC_RST_SW gpio_bit_reset(ADC_SW_PORT, ADC_SW_PIN)
uint16_t adc0_channel_read(void);
void adc0_channel_init(void);
extern float adc_buf[2]; // Buffer to store ADC values for 2.7V and 5.4V

#endif /* _adc_read_H */
