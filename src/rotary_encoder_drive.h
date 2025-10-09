#ifndef _rotary_encoder_drive_H
#define _rotary_encoder_drive_H

#include "gd32f10x.h"
#include "gd32f10x_gpio.h"

/*rotary_encoder gpio setting*/
#define RE_SW_PORT 			GPIOB  
#define RE_SW_PIN 			GPIO_PIN_11
#define RE_SW_PORT_RCU		RCU_GPIOB

#define RE_GPIO_PORT 		GPIOB  
#define RE_A_PIN 			GPIO_PIN_10
#define RE_A_PORT_RCC		RCU_GPIOB

#define RE_GPIO_PORT 	    GPIOB
#define RE_B_PIN 			GPIO_PIN_2              
#define RE_B_PORT_RCC		RCU_GPIOB

void rotary_encoder_init(void);
void rotary_encoder_poll(void);
extern volatile int32_t encoder_count; // 编码器计数值
extern uint8_t last_state; // 上一次的编码器状态
extern uint8_t re_sw_state; // 编码器按键状态

#endif // _rotary_encoder_drive_H