#ifndef _XPT2046_TOUCH_H
#define _XPT2046_TOUCH_H

#include "gd32f10x.h"
#include "gd32f10x_gpio.h"
#include <stdint.h>

// ==== 硬件定义 ====
#define XPT2046_SPI         SPI0
#define XPT2046_CS_PORT     GPIOB
#define XPT2046_CS_PIN      GPIO_PIN_5
#define XPT2046_PENIRQ_PORT GPIOB
#define XPT2046_PENIRQ_PIN  GPIO_PIN_3
#define XPT2046_BUSY_PORT    GPIOB
#define XPT2046_BUSY_PIN     GPIO_PIN_4


void xpt2046_gpio_spi_init(void);
void xpt2046_get_xy(uint16_t *x, uint16_t *y);
void xpt2046_spi_init(void);

#endif /* DEA9DE2C_BD2E_46E4_816C_E67C49FD4AD1 */
