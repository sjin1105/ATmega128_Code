#include <avr/io.h>
#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define EOS -1

unsigned char FND_SEGNP[11] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x40 }; // 0 1 2 3 4 5 6 7 8 9 -
unsigned char FND_SEGPOS[4] = { 0x01,0x02,0x04,0x08 }; // 7seg 위치
volatile int a, b, c, d;
volatile int state, tone, y, tick_time, L, i, led_state;
char f_table[8] = { 17, 43, 66, 77, 97, 114, 117, 137 };  // 음계
unsigned char LEDNP[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF }; // LED 1 2 3 4 5 6 7 8 
unsigned char adc_low, adc_high;
unsigned short value;
int song[4] = { 0, 2, 5, EOS }; // 이 부분 변경 !!!!!!!!!!!!!!
int song_1[4] = { 1, 2, EOS }; // 변경

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

	DDRB = DDRB | 0x10;

	DDRF &= 0xFE;  // ADC와 PORTF 가 연결
	ADMUX = 0x40;  // ADC 기본 설정
	ADCSR = 0x87;  // 7bit ADC Enable, 6bit ADC start, 5bit ADC freerunning, 3bit ADC interrupt Enable

	UBRR0H = 0; // 하위 4비트 사용
	UBRR0L = 0x67;  // 정의에 의해 103
	UCSR0B = 0x08; //TXEN0 = 1 송신부 활성화

	sei();

	while (1) {
		read_adc();

		SEG();
		PORTA = LEDNP[L];
		if (value < 800) {
			L++;
			a++;
			if (L == 9) {
				L = 0;
				a = 0;
				y = 0;
				do {
					tone = song[y++]; // 노래 음계
					if(led_state == 0){
						led_state = 1;
					}else led_state = 0;

					for (i = 0; i < 250; i++) {
						SEG();
						if (led_state == 0) {
							PORTA = 0xFF;
						}
						if (led_state == 1) {
							PORTA = 0x00;
						}
					}
				} while (tone != EOS); // 노래 마지막 음인지 검사
			}
			else {
				y = 0;
				do {
					tone = song_1[y++]; // 노래 음계
					_delay_ms(200); // 한 계명당 지속 시간
				} while (tone != EOS); // 노래 마지막 음인지 검사
			}
		}
	}
}

void SEG()
{
	PORTC = FND_SEGNP[a];
	PORTG = FND_SEGPOS[0];
	_delay_ms(1);
}

void read_adc()
{
	ADCSR |= 0x40;
	while ((ADCSR & 0x10) != 0x10);
	adc_low = ADCL;
	adc_high = ADCH;

	value = ((unsigned short)adc_high << 8) | (unsigned short)adc_low;
}

ISR(TIMER0_OVF_vect)  // timer interrupt
{
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
