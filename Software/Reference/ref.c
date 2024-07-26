#include <intrins.h>
#include "MAIN.h"
// #include "i2c.h"
// #include "tm1650.h"

#define MAIN_Fosc 24000000L // 定义主时钟

#define Timer0_Reload (65536UL - (MAIN_Fosc / 1000)) // Timer 0 中断频率, 1000次/秒

// bit B_1ms;          //1ms标志
u16 adcvol;

unsigned int cycle = 1000;
unsigned int pwm_low;

u16 Get_ADCResult(u8 channel);

void Init(void)
{

	P5M0 = (P5M0 & ~0x20) | 0x10;
	P5M1 &= ~0x30;
	P5DR &= ~0x10;

	P3M0 = (P3M0 & ~0x01) | 0x0e;
	P3M1 = (P3M1 & ~0x0e) | 0x01;
	P3DR &= ~0x07;

	K2 = 0;
	LEDA = 1;
	GK = 1;
	BK = 0;
	RK = 1;
	delay10ms(150);
	K2 = 1;

	P_SW2 |= 0x80;	  // 扩展寄存器(XFR)访问使能
	ADCTIM = 0x3f;	  // 设置 ADC 内部时序，ADC采样时间建议设最大值
	ADCCFG = 0x3f;	  // 设置 ADC 时钟为系统时钟/2/16/16
	ADC_CONTR = 0x80; // 使能 ADC 模块

	AUXR = 0x80; // Timer0 set as 1T, 16 bits timer auto-reload,
	TH0 = (u8)(Timer0_Reload / 256);
	TL0 = (u8)(Timer0_Reload % 256);
	ET0 = 1; // Timer0 interrupt enable
	TR0 = 1; // Tiner0 run
	EA = 1;	 // 打开总中断

	// led 蓝色呼吸
	for (pwm_low = 0; pwm_low < cycle; pwm_low++)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	LEDA = 0;
	delay10ms(10);
	RK = 1; // led 蓝绿色呼吸
	GK = 0;
	BK = 0;
	for (pwm_low = cycle; pwm_low > 0; pwm_low--)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	for (pwm_low = 0; pwm_low < cycle; pwm_low++)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	LEDA = 0;
	delay10ms(10);
	RK = 1; // led 绿色呼吸
	GK = 0;
	BK = 1;
	for (pwm_low = cycle; pwm_low > 0; pwm_low--)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	for (pwm_low = 0; pwm_low < cycle; pwm_low++)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	LEDA = 0;
	delay10ms(10);
	RK = 0; // led 黄色呼吸
	GK = 0;
	BK = 1;
	for (pwm_low = cycle; pwm_low > 0; pwm_low--)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	for (pwm_low = 0; pwm_low < cycle; pwm_low++)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	LEDA = 0;
	delay10ms(10);
	RK = 0; // led 红色呼吸
	GK = 1;
	BK = 1;
	for (pwm_low = cycle; pwm_low > 0; pwm_low--)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	for (pwm_low = 0; pwm_low < cycle; pwm_low++)
	{
		LEDA = 0;
		delay(pwm_low);
		LEDA = 1;
		delay(cycle - pwm_low);
	}
	LEDA = 0;
	GK = BK = RK = 1;
	// K2=0;
}

void main(void)
{
	unsigned int k = 0;

	Init();
	delay10ms(5);
	adcvol = Get_ADCResult(0); // 参数0~5,查询方式做一次ADC
	delay10ms(20);
	while (1)
	{
		if (adcvol <= 242) // ghost 红色灯
		{
			RK = 0;
			GK = 1;
			BK = 1;
		}
		if ((adcvol > 242) && (adcvol <= 243)) // normal 绿色灯
		{
			RK = 1;
			GK = 0;
			BK = 1;
		}
		if (adcvol > 243) // angel 蓝色灯
		{
			RK = 1;
			GK = 1;
			BK = 0;
		}

		for (pwm_low = cycle; pwm_low > 0; pwm_low--)
		{
			LEDA = 0;
			delay1(pwm_low);
			LEDA = 1;
			delay1(cycle - pwm_low);
		}
		for (pwm_low = 0; pwm_low < cycle; pwm_low++)
		{
			LEDA = 0;
			delay(pwm_low);
			LEDA = 1;
			delay(cycle - pwm_low);
		}
		LEDA = 0;
		delay10ms(20);

		k++;
		if (k == 2)
		{
			K2 = 0;
		}
	}
}

void delay10ms(uchar z)
{

	uchar i, j, s;
	s = z;
	while (s--)
	{
		i = 144;
		j = 157;
		do
		{
			while (--j)
				;
		} while (--i);
	}
}
void delay(unsigned int t)
{
	unsigned int count = 1 * t;
	while (count--)
		;
}
void delay1(unsigned int t)
{
	unsigned int count = 2 * t;
	while (count--)
		;
}

// 查询法读一次ADC结果.
u16 Get_ADCResult(u8 channel) // channel = 0~5
{
	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | channel;
	_nop_();
	_nop_();
	_nop_();
	_nop_();

	while ((ADC_CONTR & 0x20) == 0)
		; // wait for ADC finish
	ADC_CONTR &= ~0x20;
	return (((u16)ADC_RES << 8) | ADC_RESL);
}

/********************** Timer0 1ms中断函数 ************************/

/*void timer0 (void) interrupt 1
{
	B_1ms = 1;
}*/
