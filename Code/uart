#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>

void USART0_init(unsigned int UBRR0) // 9600kbps 지정
{
	UBRR0H = 0;
	UBRR0L = 0x67;
	UCSR0B = 0x08; //TXEN0 = 1
}

void TX0_ch(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));  // UDR0 에 값이 있는지 확인

	UDR0 = data;
}


void TX0_STR (unsigned char *str)  // 문자열 출력
{
	while(*str)
	{
		TX0_ch(*str++);
	}
}

int main(void)
{
	unsigned char a = 1;
	unsigned char b = '2';
	unsigned char d = '3';
	unsigned char c = '4';
	unsigned char str[10];
	sprintf(str, "player p%d\r\n %d, %d %d %d", a ,a ,b, c, d);

	USART0_init(103);

	while(1)
	{
		TX0_STR(str);
		_delay_ms(1000);
	}

/*
	while(1)
	{	
		TX0_ch(b);
		TX0_ch(c);
		TX0_ch(a);
		_delay_ms(1000);
	}
*/
}
