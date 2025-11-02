#include "gd32f10x.h"
#include <stdint.h>
#include "XPT2046_TOUCH.h"

#include "gd32f10x.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// ==== SPI与CS、PENIRQ初始化 ====
void xpt2046_gpio_init(void) 
{
    gpio_init(XPT2046_CS_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, XPT2046_CS_PIN);
    gpio_bit_set(XPT2046_CS_PORT, XPT2046_CS_PIN); // CS拉高
    gpio_init(XPT2046_PENIRQ_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, XPT2046_PENIRQ_PIN);
    gpio_init(XPT2046_BUSY_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, XPT2046_BUSY_PIN);
}

static uint8_t xpt2046_spi_rw(uint8_t data) {
    while (RESET == spi_i2s_flag_get(XPT2046_SPI, SPI_FLAG_TBE));
    spi_i2s_data_transmit(XPT2046_SPI, data);
    while (RESET == spi_i2s_flag_get(XPT2046_SPI, SPI_FLAG_RBNE));
    return spi_i2s_data_receive(XPT2046_SPI);
}

#define XPT2046_CMD_X 0xD0
#define XPT2046_CMD_Y 0x90

// ==== 读取ADC ====
static uint16_t xpt2046_read_adc(uint8_t cmd) {
    uint16_t value = 0;
    gpio_bit_reset(XPT2046_CS_PORT, XPT2046_CS_PIN);
    xpt2046_spi_rw(cmd);
    value = xpt2046_spi_rw(0x00) << 8;
    value |= xpt2046_spi_rw(0x00);
    gpio_bit_set(XPT2046_CS_PORT, XPT2046_CS_PIN);
    value >>= 4;
    return value;
}

// ==== PENIRQ检测 ====
bool xpt2046_is_pressed(void) {
    return gpio_input_bit_get(XPT2046_PENIRQ_PORT, XPT2046_PENIRQ_PIN) == RESET;
}

// ==== 中值滤波采样 ====
#define XPT2046_SAMPLE_NUM 7

static uint16_t median(uint16_t *arr, uint8_t n) {
    uint16_t tmp[XPT2046_SAMPLE_NUM];
    memcpy(tmp, arr, n * sizeof(uint16_t));
    for (uint8_t i = 0; i < n-1; i++) {
        for (uint8_t j = i+1; j < n; j++) {
            if (tmp[i] > tmp[j]) {
                uint16_t t = tmp[i]; tmp[i] = tmp[j]; tmp[j] = t;
            }
        }
    }
    return tmp[n/2];
}

static void xpt2046_get_raw_xy(uint16_t *x, uint16_t *y) {
    uint16_t x_buf[XPT2046_SAMPLE_NUM], y_buf[XPT2046_SAMPLE_NUM];
    for (uint8_t i = 0; i < XPT2046_SAMPLE_NUM; i++) {
        x_buf[i] = xpt2046_read_adc(XPT2046_CMD_X);
        y_buf[i] = xpt2046_read_adc(XPT2046_CMD_Y);
    }
    *x = median(x_buf, XPT2046_SAMPLE_NUM);
    *y = median(y_buf, XPT2046_SAMPLE_NUM);
}

// ==== 四点校准算法 ====
typedef struct {
    float kx, bx;
    float ky, by;
} xpt2046_calib_t;

static xpt2046_calib_t calib = {1,0,1,0}; // 默认线性映射

// 校准流程：分别点击屏幕左上、右上、右下、左下四角，记录原始ADC值和屏幕坐标
// 例如：
// raw[0] = {adc_x0, adc_y0}; lcd[0] = {0,0}
// raw[1] = {adc_x1, adc_y1}; lcd[1] = {LCD_W-1,0}
// raw[2] = {adc_x2, adc_y2}; lcd[2] = {LCD_W-1,LCD_H-1}
// raw[3] = {adc_x3, adc_y3}; lcd[3] = {0,LCD_H-1}
void xpt2046_calibrate(uint16_t raw[4][2], uint16_t lcd[4][2]) {
    // 只做线性校准（可扩展为仿射/投影变换）
    float kx = (float)(lcd[1][0] - lcd[0][0]) / (raw[1][0] - raw[0][0]);
    float bx = lcd[0][0] - kx * raw[0][0];
    float ky = (float)(lcd[3][1] - lcd[0][1]) / (raw[3][1] - raw[0][1]);
    float by = lcd[0][1] - ky * raw[0][1];
    calib.kx = kx; calib.bx = bx;
    calib.ky = ky; calib.by = by;
}

// ==== 坐标转换 ====
void xpt2046_get_xy(uint16_t *lcd_x, uint16_t *lcd_y) {
    uint16_t x, y;
    xpt2046_get_raw_xy(&x, &y);
    int32_t tx = (int32_t)(calib.kx * x + calib.bx);
    int32_t ty = (int32_t)(calib.ky * y + calib.by);
    // 限制范围
    if (tx < 0) tx = 0;
    if (ty < 0) ty = 0;
    if (tx > 127) tx = 127; // 以128x96为例
    if (ty > 95) ty = 95;
    *lcd_x = tx;
    *lcd_y = ty;
}

// ==== 触摸事件结构体 ====
typedef enum {
    TOUCH_NONE = 0,
    TOUCH_DOWN,
    TOUCH_MOVE,
    TOUCH_UP
} touch_state_t;

typedef struct {
    touch_state_t state;
    uint16_t x;
    uint16_t y;
} touch_event_t;

// ==== 事件处理（带去抖动和滑动检测） ====
void xpt2046_touch_scan(touch_event_t *event) {
    static bool last_pressed = false;
    static uint16_t last_x = 0, last_y = 0;
    static uint8_t stable_cnt = 0;

    if (xpt2046_is_pressed()) {
        uint16_t x, y;
        xpt2046_get_xy(&x, &y);

        if (stable_cnt == 0 || (abs(x - last_x) < 5 && abs(y - last_y) < 5)) {
            stable_cnt++;
            last_x = x;
            last_y = y;
        } else {
            stable_cnt = 0;
        }

        if (stable_cnt > 2) {
            if (!last_pressed) {
                event->state = TOUCH_DOWN;
            } else {
                event->state = TOUCH_MOVE;
            }
            event->x = last_x;
            event->y = last_y;
            last_pressed = true;
        }
    } else {
        if (last_pressed) {
            event->state = TOUCH_UP;
            event->x = last_x;
            event->y = last_y;
        } else {
            event->state = TOUCH_NONE;
        }
        stable_cnt = 0;
        last_pressed = false;
    }
}

// ==== 典型主循环调用 ====
// touch_event_t touch;
// xpt2046_gpio_spi_init();
// while (1) {
//     xpt2046_touch_scan(&touch);
//     switch (touch.state) {
//         case TOUCH_DOWN: /* 按下事件 */ break;
//         case TOUCH_MOVE: /* 滑动事件 */ break;
//         case TOUCH_UP:   /* 抬起事件 */ break;
//         default: break;
//     }
// }
