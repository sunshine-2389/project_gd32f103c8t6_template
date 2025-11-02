/*!
    \file  main.c
    \brief USART printf
    
    \version 2014-12-26, V1.0.0, firmware for GD32F10x
    \version 2017-06-20, V2.0.0, firmware for GD32F10x
    \version 2018-07-31, V2.1.0, firmware for GD32F10x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f10x.h"
#include <stdio.h>
#include <stdbool.h>
#include <systick.h>
#include "spi.h"
#include "ST7571_12896Drive.h"
#include "rotary_encoder_drive.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void init()
{
    /* configure systick */
    rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV1);
    systick_config();
    rcu_periph_clock_enable(RCU_AF);
    //gpio_pin_remap_config(GPIO_SWJ_NONJTRST_REMAP, ENABLE);//ÏÂÔØ¿ÚNJTRSTÒý½Åµ±×öÆÕÍ¨I/O¿Ú
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);//PB4ÅäÖÃ³ÉÊä³ö
	gpio_bit_reset(GPIOB, GPIO_PIN_7);
    /* enable GPIOB clock */
   /*rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_bit_reset(GPIOB, GPIO_PIN_7);*/
    /* configure the GPIO port */ 
    spi0_init();
    LCD_PROT_Init();
    LCD_Init();
    lcd_2dp(gImage_step2bit);
    xpt2046_gpio_spi_init();
    
}


int main(void)
{
    init();
    /* enable GPIO clock */
    while(1)
    {
        gpio_bit_set(GPIOB, GPIO_PIN_7);
        //lcd_2dp(gImage_step2bit);
        //delay_1us(10);
        delay_1ms(50);
        //lcd_display(16,16,1,16,byte_2bit+64);
        gpio_bit_reset(GPIOB, GPIO_PIN_7);
        delay_1ms(50);
        //delay_1us(10);
    }
}

