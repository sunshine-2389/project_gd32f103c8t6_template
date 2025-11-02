#ifndef _spi_H
#define _spi_H

#include "gd32f10x.h"
#include "gd32f10x_gpio.h"
#include "gd32f10x_rcu.h"
#include "gd32f10x_spi.h"

/*spi gpio setting*/
#define SPI_NSS_PIN          GPIO_PIN_4
#define SPI_SCK_PIN         GPIO_PIN_5
#define SPI_MISO_PIN        GPIO_PIN_6
#define SPI_MOSI_PIN        GPIO_PIN_7
#define SPI_GPIO_PORT       GPIOA
#define SPI_PORT            SPI0

#define SPI_SET_CS gpio_bit_set(SPI_GPIO_PORT, SPI_NSS_PIN)
#define SPI_RST_CS gpio_bit_reset(SPI_GPIO_PORT, SPI_NSS_PIN)
#define SPI_SET_SCK gpio_bit_set(SPI_GPIO_PORT, SPI_SCK_PIN)
#define SPI_RST_SCK gpio_bit_reset(SPI_GPIO_PORT, SPI_SCK_PIN)
#define SPI_SET_MISO gpio_bit_set(SPI_GPIO_PORT, SPI_MISO_PIN)
#define SPI_RST_MISO gpio_bit_reset(SPI_GPIO_PORT, SPI_MISO_PIN)
#define SPI_SET_MOSI gpio_bit_set(SPI_GPIO_PORT, SPI_MOSI_PIN)
#define SPI_RST_MOSI gpio_bit_reset(SPI_GPIO_PORT, SPI_MOSI_PIN)

void spi0_init(void);
uint8_t spi0_write_byte(uint8_t data);
uint8_t spi0_read_byte(void);
void spi0_write_buffer(uint8_t *tx_buf, uint16_t len);
void spi0_read_buffer(uint8_t *rx_buf, uint16_t len);


#endif /* _spi_H */