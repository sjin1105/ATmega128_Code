#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#define EOS -1

unsigned char FND_SEGNP[11] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x40 }; // 0 1 2 3 4 5 6 7 8 9 -
unsigned char FND_SEGPOS[4] = { 0x01,0x02,0x04,0x08 }; // 7seg 위치
unsigned char SEGPLAY[1] = { 0x40 };

unsigned char LEDNP[11] = { 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80 }; // LED 1 2 3 4

char f_table[8] = { 17, 43, 66, 77, 97, 114, 117, 137 };  // 음계
int song[2] = { 8, EOS }; // 계명
int song_1[3] = { 2, 3, EOS }; // 계명

volatile unsigned int state, y, tone, nand, random1, p_value, num, adc_state;
volatile unsigned char a, b, c, d;

volatile unsigned char adc_low, adc_high;
volatile unsigned short value;

void SEG();

int main()
{
	DDRC = 0xFF;  // 7 SEG POWER
	DDRG |= 0x0F;

	EICRA = 0x00;
	EICRB = 0x0A;  // 4, 5 interrupt 하강엣지
	EIMSK = 0x30;  // 4, 5 interrupt 활성화

	DDRA = 0xFF;  // LED POWER

	TCNT0 = 0;  // 타이머
	TIMSK = 0x01;  // nomal mode timer

	TCCR0 = 0x03;  // 32분주

	DDRB = DDRB | 0x10;  // 버튼

	DDRF &= 0xFE;  // ADC와 PORTF 가 연결
	ADMUX = 0x40;  // ADC 기본 설정
	ADCSR = 0xEF;  // 7bit ADC Enable, 6bit ADC start, 5bit ADC freerunning, 3bit ADC interrupt Enable, 4bit ADC interrupt flag

	cli();

	while (1) {
		if (((PINE & 0x10) == 0x00) || ((PINE & 0x20) == 0x00)) {
			break;
		}
		_delay_ms(1000);
	}

	sei();

	adc_state = 0;

	while (1) {
		num = 5;
		y = 0;
		do {
			tone = song_1[y++]; // 노래 음계
			_delay_ms(200); // 한 계명당 지속 시간
		} while (tone != EOS); // 노래 마지막 음인지 검사
		while (1) {
			PORTA = LEDNP[num];
			SEG();
			if (num <= 1) {
				d++;
				num = 5;
				y = 0;
				do {
					tone = song[y++]; // 노래 음계
					_delay_ms(200); // 한 계명당 지속 시간
				} while (tone != EOS); // 노래 마지막 음인지 검사
			}
			if (num >= 9) {
				a++;
				num = 5;
				y = 0;
				do {
					tone = song[y++]; // 노래 음계
					_delay_ms(200); // 한 계명당 지속 시간
				} while (tone != EOS); // 노래 마지막 음인지 검사
			}
			if ((a == 5) || (d == 5)) {
				a = 0;
				d = 0;
				break;
			}
		}
	}
}

void SEG()
{
	PORTC = FND_SEGNP[a];
	PORTG = FND_SEGPOS[0];
	_delay_ms(1);

	PORTC = SEGPLAY[0];
	PORTG = FND_SEGPOS[1];
	_delay_ms(1);

	PORTC = SEGPLAY[0];
	PORTG = FND_SEGPOS[2];
	_delay_ms(1);

	PORTC = FND_SEGNP[d];
	PORTG = FND_SEGPOS[3];
	_delay_ms(1);
}

ISR(INT4_vect) // sw1 interrupt 모드 변경
{
	if (adc_state == 1) {
		p_value = (rand() % 2) + 1;
		num -= p_value;
		adc_state = 0;
	}
}

ISR(INT5_vect)  // sw2 interrupt 초기화
{
	if (adc_state == 1) {
		p_value = (rand() % 2) + 1;
		num += p_value;
		adc_state = 0;
	}
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

	random1++;
	if (random1 == 10000000) {
		random1 = 0;
	}
	srand(random1);
}

ISR(ADC_vect)  // adc interrupt
{
	adc_low = ADCL;
	adc_high = ADCH;

	value = ((unsigned short)adc_high << 8) | (unsigned short)adc_low;

	if (value < 800) {
		adc_state = 1;
	}
}
