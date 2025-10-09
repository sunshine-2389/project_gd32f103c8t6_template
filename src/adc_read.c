#include "gd32f10x.h"
#include "adc_read.h"

#include "gd32f10x_adc.h"
#include "gd32f10x_gpio.h"

float adc_buf[2] = {0}; // Buffer to store ADC values for 2.7V and 5.4V
// ADC初始化（以ADC0通道1/PA1为例）
void adc0_channel_init(void) {
    // 1. 使能GPIOA和ADC0时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_ADC0);

    // 2. 配置PA1,PA2为模拟输入
    gpio_init(ADC_27_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_27_PIN);
    gpio_init(ADC_54_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_54_PIN);
    gpio_init(ADC_SW_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, ADC_SW_PIN); // ADC_SW引脚配置为模拟输入

    // 3. ADC配置
    adc_deinit(ADC0);// 复位ADC0寄存器
    adc_mode_config(ADC_MODE_FREE);// 设置ADC工作模式为独立模式
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);// 设置数据对齐方式为右对齐
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2); // 设置常规通道长度为2
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_1, ADC_SAMPLETIME_55POINT5);// 配置通道1（PA1）为第0个通道，采样时间为55.5个时钟周期
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_2, ADC_SAMPLETIME_55POINT5);// 配置通道2（PA2）为第1个通道，采样时间为55.5个时钟周期

    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_T7_TRGO);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    adc_enable(ADC0);
    // 4. 校准
    adc_calibration_enable(ADC0);
    ADC_RST_SW; // 设置ADC_SW引脚为高电平
}

// 读取ADC0通道1的值
uint16_t adc0_channel_read(void) 
{
    // 触发ADC转换
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    for (int i = 0; i < ADC_CHANNEL_NUM; i++) {
            while (!adc_flag_get(ADC0, ADC_FLAG_EOC));
            adc_buf[i] = adc_regular_data_read(ADC0);
        }
}


