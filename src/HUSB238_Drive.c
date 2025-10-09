#include "gd32f10x.h"
#include <stdint.h>
#include "gd32f10x_i2c.h"
#include "gd32f10x_gpio.h"
#include "HUSB238_Drive.h"


// --- I2C初始化 ---
void husb238_i2c_init(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C1);

    gpio_init(HUSB238_I2C_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, HUSB238_I2C_SCL_PIN | HUSB238_I2C_SDA_PIN);

    i2c_deinit(HUSB238_I2C);
    i2c_clock_config(HUSB238_I2C, 100000, I2C_DTCY_2); // 100kHz
    i2c_mode_addr_config(HUSB238_I2C, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0);
    i2c_enable(HUSB238_I2C);
}

// --- I2C写寄存器 ---
void husb238_write_reg(uint8_t reg, uint8_t data) {
    while(i2c_flag_get(HUSB238_I2C, I2C_FLAG_I2CBSY));
    i2c_start_on_bus(HUSB238_I2C);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_SBSEND));
    i2c_master_addressing(HUSB238_I2C, HUSB238_ADDR << 1, I2C_TRANSMITTER);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_ADDSEND));
    i2c_flag_clear(HUSB238_I2C, I2C_FLAG_ADDSEND);

    i2c_data_transmit(HUSB238_I2C, reg);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_TBE));
    i2c_data_transmit(HUSB238_I2C, data);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_TBE));

    i2c_stop_on_bus(HUSB238_I2C);
    while(I2C_CTL0(HUSB238_I2C) & 0x0200);
}

// --- I2C读寄存器 ---
uint8_t husb238_read_reg(uint8_t reg) {
    uint8_t data = 0;
    while(i2c_flag_get(HUSB238_I2C, I2C_FLAG_I2CBSY));
    i2c_start_on_bus(HUSB238_I2C);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_SBSEND));
    i2c_master_addressing(HUSB238_I2C, HUSB238_ADDR << 1, I2C_TRANSMITTER);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_ADDSEND));
    i2c_flag_clear(HUSB238_I2C, I2C_FLAG_ADDSEND);

    i2c_data_transmit(HUSB238_I2C, reg);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_TBE));

    i2c_start_on_bus(HUSB238_I2C);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_SBSEND));
    i2c_master_addressing(HUSB238_I2C, HUSB238_ADDR << 1, I2C_RECEIVER);
    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_ADDSEND));
    i2c_flag_clear(HUSB238_I2C, I2C_FLAG_ADDSEND);

    while(!i2c_flag_get(HUSB238_I2C, I2C_FLAG_RBNE));
    data = i2c_data_receive(HUSB238_I2C);

    i2c_stop_on_bus(HUSB238_I2C);
    while(I2C_CTL0(HUSB238_I2C) & 0x0200);
    return data;
}

// --- 设置输出电压/电流（举例，具体寄存器请查HUSB238手册） ---
void husb238_set_voltage(uint8_t v_sel) {
    // 假设0x03为电压选择寄存器
    husb238_write_reg(0x08, v_sel);
}


// --- 读取状态寄存器 ---
uint8_t husb238_get_status(void) {
    // 假设0x00为状态寄存器
    return husb238_read_reg(0x00);
}

// --- 示例用法 ---
// husb238_i2c_init();
// husb238_set_voltage(0x02); // 选择某一电压档
// husb238_set_current(0x01); // 选择某一电流档
// uint8_t status = husb238_get_status();
