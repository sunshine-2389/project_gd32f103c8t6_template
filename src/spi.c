#include "gd32f10x.h"
#include "spi.h"

void spi0_init(void) {
    // 使能时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SPI0);

    // 配置SCK、MOSI为复用推挽输出，MISO为浮空输入，NSS为通用推挽输出
    gpio_init(SPI_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, SPI_SCK_PIN | SPI_MOSI_PIN);
    gpio_init(SPI_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, SPI_MISO_PIN);
    gpio_init(SPI_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SPI_NSS_PIN);

    // NSS拉高
    gpio_bit_set(SPI_GPIO_PORT, SPI_NSS_PIN);
    spi_i2s_deinit(SPI_PORT);
    // SPI参数配置
    spi_parameter_struct spi_init_struct;
    spi_struct_para_init(&spi_init_struct);
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_8; // 分频，视外设需求调整
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI_PORT, &spi_init_struct);

    spi_enable(SPI_PORT);
}

// ...existing code...

// SPI写入一个字节
uint8_t spi0_write_byte(uint8_t data) {
    // 等待发送缓冲区空
    while (RESET == spi_i2s_flag_get(SPI_PORT, SPI_FLAG_TBE));
    spi_i2s_data_transmit(SPI_PORT, data);
}

// SPI读取一个字节
uint8_t spi0_read_byte(void) {
    // 等待接收缓冲区非空
    while (RESET == spi_i2s_flag_get(SPI_PORT, SPI_FLAG_RBNE));
    return spi_i2s_data_receive(SPI_PORT);
}

// SPI多字节写入
void spi0_write_buffer(uint8_t *tx_buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        spi0_write_byte(tx_buf[i]);
    }
}

// SPI多字节读取
void spi0_read_buffer(uint8_t *rx_buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        rx_buf[i] = spi0_read_byte();
    }
}

// ...existing code...