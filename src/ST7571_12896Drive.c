#include "ST7571_12896Drive.h"
#include "SysTick.h"
#include "spi.h"

/*2.3" lcd  drive ST7571 128*96
	G12896-08-RTP
*/

#define uchar unsigned char

#define Page_Add 0xb0	/*Value=b0~bf*/
#define Column_Add 0x00	/*Value=0~127*/
#define COM_Add 0x00	/*Value=0~127*/
#define LINE_Add 0x00	/*Value=0~127*/
#define display_on 0xaf
#define display_off 0xae

uint8_t seg1,seg2;

#include "gd32f10x_gpio.h"
#include "gd32f10x_rcu.h"
#include "gd32f10x_usart.h"

void user_RE_init(uint32_t g_port,uint32_t pin,uint32_t gpio_clock, uint32_t mode) 
{
    /* 使能GPIOB时钟 */
    rcu_periph_clock_enable(gpio_clock);
    /* 配置引脚工作模式 */
    gpio_init(g_port, mode, GPIO_OSPEED_50MHZ, pin);
    gpio_bit_reset(g_port, pin);
}

void LCD_PROT_Init(void)
{
	user_RE_init(LCD_RESET_PORT, LCD_RESET_PIN, LCD_RESET_PORT_RCC, GPIO_MODE_OUT_PP); // 初始化LCD_RESET引脚
	user_RE_init(LCD_CD_PORT, LCD_CD_PIN, LCD_CD_PORT_RCC, GPIO_MODE_OUT_PP); // 初始化LCD_CD引脚
}

void LCD_Writecom(uint8_t cmdcode)
{
	LCD_RST_CD;
	//__asm__ volatile("nop;nop;nop");
	spi0_write_byte(cmdcode);
	//__asm__ volatile("nop;nop;nop");
}

void LCD_Writedata(uint8_t data)
{
	LCD_SET_CD;
	//__asm__ volatile("nop;nop;nop");
	spi0_write_byte(data);
	//__asm__ volatile("nop;nop;nop");
}

void LCD_FillSreen()
{
	uchar j,k;
	for(k=0;k<16;k++)
	{
		LCD_Writecom(Page_Add+k);
		delay_1us(1);
		/*Page low 4bit*/
		for(j=0;j<128;j++)
          LCD_Writedata(0xff);
		//for(j=0;j<128;j++)
          //LCD_Writedata(0x00);
	}
}

void LCD_Init(void)
{
	LCD_RST_RESET;
	delay_1ms(100);
	LCD_SET_RESET;
	delay_1ms(10);
	LCD_Writecom(0xe2);/*Software reset*/
	delay_1ms(10);
	LCD_Writecom(display_off);/*D=0: Display OFF*/
	LCD_Writecom(0x38);/*Set Mode*/
	LCD_Writecom(0x94);/*FR(3:0)=1001b: Frame Frequency=75Hz ±20%; BE(1:0)=01b:Booster Efficiency Level 2*/
	LCD_Writecom(0xa0);/*Set SEG Scan Direction,MX=0: Normal direction*/
	LCD_Writecom(0xc8);/*Set COM Scan Direction,MY=1: Reverse direction*/
	LCD_Writecom(0x44);/*Set COM0*/
	LCD_Writecom(COM_Add);/*C(6:0)=0b,Initial COM0=COM0*/
	LCD_Writecom(0x40);/*Set Display Start Line*/
	LCD_Writecom(LINE_Add);/*S(6:0)=0b,Line address=0*/
	LCD_Writecom(0xab);/*Turn ON internal Oscillator */
	LCD_Writecom(0x27);/*Select internal Regulator resistor 1+ (Rb / Ra)=7.2*/
	LCD_Writecom(0x81);/*2-byte instruction. Select EV for internal Regulator’s reference */
	LCD_Writecom(30);/*EV Value=30*/
	LCD_Writecom(0x57);/*Select LCD bias =1/12*/
	LCD_Writecom(0x48);/*2-byte instruction. Set display duty*/	
	LCD_Writecom(0x61);/*Set display duty=1/97*/
	LCD_Writecom(0x2c);/*Set internal power ON/OFF,Internal voltage converter circuit  on,Internal voltage regulator circuit off,Internal voltage follower circuit off*/
	LCD_Writecom(0x7b);/*te3=1,EXTENSION COMMAND SET 3*/
	LCD_Writecom(0x11);/*B/G=1,Black/White mode; */
	LCD_Writecom(0x00);/*Return normal mode*/
	LCD_Writecom(0xa6);/*Set Reverse Display REV=0, normal display*/
	LCD_Writecom(0xa4);/*Set All Pixel Normal Display ALLON=0, normal display*/
	delay_1ms(200);
	LCD_Writecom(0x2e);/*Set internal power ON/OFF,Internal voltage converter circuit  on,Internal voltage regulator circuit on,Internal voltage follower circuit off*/
	delay_1ms(200);
	LCD_Writecom(0x2f);/*Set internal power ON/OFF,Internal voltage converter circuit  on,Internal voltage regulator circuit on,Internal voltage follower circuit on*/
	delay_1ms(10);
	LCD_Writecom(display_on);/*D=1: Display ON*/
	LCD_FillSreen();
	
}




void lcd_display(uint8_t Length_y,uint8_t Width_x,uint8_t page,uint8_t column,const uchar *dp)
{	
	uchar page_count=Length_y/8;
	uchar page_actual=Page_Add+page;
	uint8_t i,j;
	LCD_Writecom(page_actual);
	LCD_Writecom(0x00|((column)&0x0f));
	LCD_Writecom(0x10|(((column)&0xf0)>>4));
	//LCD_Ending(page+C,column+Width_x);
	for(j=0;j<page_count;j++)
	{
		LCD_Writecom(page_actual+j);
		__asm__ volatile("nop");
		LCD_Writecom(0x00|((column)&0x0f));
		LCD_Writecom(0x10|(((column)&0xf0)>>4));
		delay_1us(1);
		for(i=0;i<Width_x;i++)
		{
			LCD_Writedata(~*dp);
			dp++;
		}
	}
}

void lcd_2dp(const uchar *dp)
{
	//LCD_Writecom(Page_Add);
	//LCD_Writecom(0x00|((Column_Add)&0x0f));
	//LCD_Writecom(0x10|(((Column_Add)&0xf0)>>4));
	//delay_1us(1);
	uint8_t i,j;
	for(j=0;j<12;j++)
	{
		LCD_Writecom(Page_Add+j-1);
		delay_1us(1);
		LCD_Writecom(0x00|((Column_Add+120)&0x0f));
		LCD_Writecom(0x10|(((Column_Add+120)&0xf0)>>4));
		delay_1us(1);
		for(i=0;i<128;i++)
		{
			//LCD_Writecom(0x00|((Column_Add+(i*4))&0x0f));
			//LCD_Writecom(0x10|(((Column_Add+(i*4))&0x30)>>4));
			LCD_Writedata(~*dp);
			dp++;
			delay_1us(1);
		}
	}
}

void byte_byte(const uchar *dp,const uchar *dp1)
{
	uint8_t i;
	seg1=0x00;
	seg2=0x00;
	for(i=0;i<4;i++)
	{
		seg1=seg1|(((*dp)&(1<<((2*i)+1)))>>(i+1));
		seg1=seg1|(((*dp1)&(1<<((2*i)+1)))<<(3-i));
		seg2=seg2|(((*dp)&(1<<(2*i)))>>i);
		seg2=seg2|(((*dp1)&(1<<(2*i)))>>(4-i));
	}
}

void lcd_tdp(const uchar *dp)
{
	uint8_t i,j;
	LCD_Writecom(0x00|((Column_Add)&0x0f));
	LCD_Writecom(0x10|(((Column_Add)&0xf0)>>4));
	for(j=0;j<12;j++)
	{
		LCD_Writecom(Page_Add+j);
			for(i=0;i<128;i++)
			{
				byte_byte(dp,dp+128);
				LCD_Writedata(~seg1);
				LCD_Writedata(~seg2);
				dp++;
			}
			dp=dp+128;
	}
}

void display_poweroff()
{
	LCD_Writecom(0xa9);
	delay_1ms(250);
}