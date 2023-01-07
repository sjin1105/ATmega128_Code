#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define EOS -1

unsigned char FND_SEGNP[11] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x40 }; // 0 1 2 3 4 5 6 7 8 9 -
unsigned char FND_SEGPOS[4] = { 0x01,0x02,0x04,0x08 }; // 7seg 위치
volatile int a, b, c, d;
volatile int state, tone, y, tick_time, L, timer_state;
unsigned char adc_low, adc_high;
unsigned short value;
char f_table[8] = { 17, 43, 66, 77, 97, 114, 117, 137 };  // 음계
int song[2] = { 8, EOS }; // 떙 계명
int song_1[2] = { 3, EOS };
unsigned char LEDNP[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF }; // LED 1 2 3 4

void SEG();
void read_adc();

int main()
{

	DDRC = 0xFF;  // 7 SEG POWER
	DDRG |= 0x0F;

	DDRA = 0xFF;  // LED POWER

	TCNT0 = 0;  // 타이머
	TIMSK = 0x01;  // nomal mode timer

	TCCR0 = 0x03;  // 32분주

	DDRB = DDRB | 0x10;  // 버튼

	DDRF &= 0xFE;  // ADC와 PORTF 가 연결
	ADMUX = 0x40;  // ADC 기본 설정
	ADCSR = 0x87;  // 7bit ADC Enable, 6bit ADC start, 5bit ADC freerunning, 3bit ADC interrupt Enable

	sei();

	while (1) {

		while (1) {
			if (L == 9) {
				L = 0;
			}
			PORTA = LEDNP[L];
			if (((PINE & 0x10) == 0x00) || ((PINE & 0X20) == 0x00)) {
				break;
			}
		}

		d = 9, c = 5, a = L + 1, timer_state = 0;
		while (1) {
			SEG();
			read_adc();
			if (value < 800) {
				y = 0;  //딩동댕 부저작동
				do {
					tone = song_1[y++]; // 노래 음계
					_delay_ms(200); // 한 계명당 지속 시간
				} while (tone != EOS); // 노래 마지막 음인지 검사
			}
			if (timer_state == 1) {
				L++;
				y = 0;  //딩동댕 부저작동
				do {
					tone = song[y++]; // 노래 음계
					_delay_ms(200); // 한 계명당 지속 시간
				} while (tone != EOS); // 노래 마지막 음인지 검사
				break;
			}
		}
	}
}

void read_adc()
{
	ADCSR |= 0x40;
	while ((ADCSR & 0x10) != 0x10);
	adc_low = ADCL;
	adc_high = ADCH;

	value = ((unsigned short)adc_high << 8) | (unsigned short)adc_low;
}

void SEG()
{
	PORTC = FND_SEGNP[d];
	PORTG = FND_SEGPOS[0];
	_delay_ms(1);

	PORTC = FND_SEGNP[c];
	PORTG = FND_SEGPOS[1];
	_delay_ms(1);

	PORTC = FND_SEGNP[b];
	PORTG = FND_SEGPOS[2];
	_delay_ms(1);

	PORTC = FND_SEGNP[a];
	PORTG = FND_SEGPOS[3];
	_delay_ms(1);
}

ISR(TIMER0_OVF_vect)  // timer interrupt
{
	TCNT0 = 256 - 125;
	tick_time++;
	if (tick_time == 400) {
		if (d == 0) {
			d = 9;
			c--;
		}

		else {
			d--;
		}
		tick_time = 0;
	}
	if (d == 0 && c == 0){
		timer_state = 1;
	}
	if (y != 0) {
		if (tone != EOS)  //마지막 음계
		{
			if (state == 0)
			{
				PORTB |= 1 << 4; // 버저 포트 ON
				state = 1;
			}
			else
			{
				PORTB &= ~(1 << 4); // 버저 포트 OFF
				state = 0;
			}
			TCNT0 = f_table[tone];
		}
	}
}
