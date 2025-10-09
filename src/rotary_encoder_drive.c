#include "rotary_encoder_drive.h"
#include "SysTick.h"

#include "gd32f10x.h"
#include "rotary_encoder_drive.h"

// 编码器初始化
void rotary_encoder_init(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(RE_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, RE_A_PIN | RE_B_PIN);
    gpio_init(RE_SW_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, RE_SW_PIN);
}

// 编码器状态与计数
uint8_t last_state = 0;
volatile int32_t encoder_count = 0;

// 编码器轮询处理函数，建议在定时器中断或主循环中周期性调用
void rotary_encoder_poll(void) {
    uint8_t a = gpio_input_bit_get(RE_GPIO_PORT, RE_A_PIN);
    uint8_t b = gpio_input_bit_get(RE_GPIO_PORT, RE_B_PIN);
    uint8_t state = (a << 1) | b;

    if (state != last_state) {
        // 仅在状态变化时判断方向
        if (((last_state == 0x0) && (state == 0x1)) ||
            ((last_state == 0x1) && (state == 0x3)) ||
            ((last_state == 0x3) && (state == 0x2)) ||
            ((last_state == 0x2) && (state == 0x0))) {
            encoder_count++; // 顺时针
        } else if (((last_state == 0x0) && (state == 0x2)) ||
                   ((last_state == 0x2) && (state == 0x3)) ||
                   ((last_state == 0x3) && (state == 0x1)) ||
                   ((last_state == 0x1) && (state == 0x0))) {
            encoder_count--; // 逆时针
        }
        last_state = state;
    }
}