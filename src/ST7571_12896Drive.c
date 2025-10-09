#include "ST7571_12896Drive.h"
#include "SysTick.h"
#include "spi.h"

/*2.3" lcd  drive ST7571 128*96
	G12896-08-RTP
*/

#define uchar unsigned char

#define Page_Add 0xb0
#define Column_Add 0x00

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
	spi1_write_byte(cmdcode);
}



void LCD_Writedata(uint8_t data)
{
	LCD_SET_CD;
	spi1_write_byte(data);
}

void LCD_FillSreen()
{
	uchar j,k;
	for(k=0;k<12;k++)
	{
		LCD_Writecom(Page_Add+k);
		/*Page low 4bit*/
		for(j=0;j<128;j++)
          LCD_Writedata(0x00);
		for(j=0;j<128;j++)
          LCD_Writedata(0x00);
	}
}

void LCD_Init(void)
{
	LCD_RST_RESET;
	delay_1ms(1);
	LCD_SET_RESET;
	delay_1ms(10);
	LCD_Writecom(0xe2);/*Software reset*/
	delay_1ms(10);
	LCD_Writecom(0xae);/*D=0: Display OFF*/
	LCD_Writecom(0x38);/*Set Mode*/
	LCD_Writecom(0x94);/*FR(3:0)=1001b: Frame Frequency=75Hz ±20%; BE(1:0)=01b:Booster Efficiency Level 2*/
	LCD_Writecom(0xa0);/*Set SEG Scan Direction,MX=0: Normal direction*/
	LCD_Writecom(0xc8);/*Set COM Scan Direction,MY=1: Reverse direction*/
	LCD_Writecom(0x44);/*Set COM0*/
	LCD_Writecom(0x00);/*C(6:0)=0b,Initial COM0=COM0*/
	LCD_Writecom(0x40);/*Set Display Start Line*/
	LCD_Writecom(0x00);/*S(6:0)=0b,Line address=0*/
	LCD_Writecom(0xab);
	LCD_Writecom(0x27);
	LCD_Writecom(0x81);
	LCD_Writecom(30);
	LCD_Writecom(0x57);
	LCD_Writecom(0x48);
	LCD_Writecom(0x61);
	LCD_Writecom(0x2c);
	delay_1ms(10);
	LCD_Writecom(0x2e);
	delay_1ms(10);
	LCD_Writecom(0x2f);
	delay_1ms(1);
	LCD_Writecom(0x7b);/*te3=1,EXTENSION COMMAND SET 3*/
	LCD_Writecom(0x10);/*B/G=0,Gray mode (default)*/
	LCD_Writecom(0x00);/*Return normal mode*/
	LCD_Writecom(0xa6);
	LCD_Writecom(0xa4);
	LCD_Writecom(0xaf);/*D=1: Display ON*/
	LCD_FillSreen();
}


void LCD_SetPos(uint8_t page,uint8_t column)
{
	column=column-1;
	page=page-1;
	LCD_Writecom(0xf4);
	LCD_Writedata(column);
	LCD_Writecom(0xf5);
	LCD_Writedata(page);
}

void LCD_Ending(uint8_t page,uint8_t column)
{
	column=column-1;
	page=page-1;
	LCD_Writecom(0xf6);
	LCD_Writedata(column);
	LCD_Writecom(0xf7);
	LCD_Writedata(page);
}

void lcd_display(uint8_t Length_y,uint8_t Width_x,uint8_t page,uint8_t column,uchar fs,const uchar *dp)
{	
	uchar C=Length_y/4;
	uint8_t i,j;
	LCD_Writecom(0xf9);
	LCD_SetPos(page,column);
	LCD_Ending(page+C,column+Width_x);
	for(j=0;j<C;j++)
	{
		LCD_Writecom(0x60|((Page_Add+j)&0x0f));
		LCD_Writecom(0x70|(((Page_Add+j)&0x30)>>4));
		for(i=0;i<Width_x;i++)
		{
			if(fs==0)
			{
				LCD_Writecom(0x01);
				LCD_Writedata( *dp);
			}  
			if(fs==1)
			{
				LCD_Writecom(0x01);
				LCD_Writedata(~*dp);
			}
			dp++;
		}
	}
	LCD_Writecom(0xf8);
}

void lcd_2dp(uchar fs,const uchar *dp)
{
	uint8_t i,j;
	for(j=0;j<12;j++)
	{
		LCD_Writecom(Page_Add+j);
				for(i=0;i<128;i++)
			{
				//LCD_Writecom(0x00|((Column_Add+(i*4))&0x0f));
				//LCD_Writecom(0x10|(((Column_Add+(i*4))&0x30)>>4));
				if(fs==0)
				{
					LCD_Writedata( *dp);
				}  
				if(fs==1)
				{
					LCD_Writedata(~*dp);
				}
				dp++;
			}
	}
}

void lcd_4dp(uchar fs,const uchar *dp)
{
	uint8_t i,j,k;
	for(j=0;j<12;j++)
	{
		LCD_Writecom(Page_Add+j);
		for(k=0;k<2;k++)
		{
				for(i=0;i<128;i++)
			{
				if(fs==0)
				{
					LCD_Writedata( *dp);
				}  
				if(fs==1)
				{
					LCD_Writedata(~*dp);
				}
				dp++;
			}
		}
	}
}

void lcd_4tdp(uchar fs,const uchar *dp)
{
	uint8_t i,j,k;
	LCD_Writecom(0x00|((Column_Add)&0x0f));
	LCD_Writecom(0x10|(((Column_Add)&0x30)>>4));
	for(j=0;j<12;j++)
	{
		LCD_Writecom(Page_Add+j);
		for(k=0;k<8;k++)
		{	for(i=0;i<32;i++)
			{
				if(fs==0)
				{
					LCD_Writedata(*(dp+32));
					LCD_Writedata( *dp);
				}  
				if(fs==1)
				{
					LCD_Writedata(~*(dp+32));
					LCD_Writedata(~*dp);
				}
				dp++;
			}
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

void lcd_tdp(uchar fs,const uchar *dp)
{
	uint8_t i,j;
	LCD_Writecom(0x00|((Column_Add)&0x0f));
	LCD_Writecom(0x10|(((Column_Add)&0x30)>>4));
	for(j=0;j<12;j++)
	{
		LCD_Writecom(Page_Add+j);
			for(i=0;i<128;i++)
			{
				if(fs==0)
				{
					byte_byte(dp,dp+128);
					LCD_Writedata(seg1);
					LCD_Writedata(seg2);
				}  
				if(fs==1)
				{
					byte_byte(dp,dp+128);
					LCD_Writedata(~seg1);
					LCD_Writedata(~seg2);
				}
				dp++;
			}
			dp=dp+128;
	}
}

