#include <avr/io.h>
#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define EOS -1

unsigned char FND_SEGNP[11] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x40 }; // 0 1 2 3 4 5 6 7 8 9 -
unsigned char FND_SEGPOS[4] = { 0x01,0x02,0x04,0x08 }; // 7seg 위치

unsigned char LEDNP[5] = { 0x00,0x01,0x03,0x07,0x0F }; // LED 1 2 3 4

char f_table[8] = { 17, 43, 66, 77, 97, 114, 117, 137 };  // 음계
int song[2] = { 8, EOS }; // 계명
int song_1[2] = { 2, EOS }; // 계명

volatile unsigned int state, y, tone, value, state_value, BUZZER_STATE, tick_time, bu_state;
volatile unsigned char a, b, c, d;

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

	sei();
	
	value = 60;

	while (1) {
		d = value % 10;
		c = value / 10 % 10;
		b = value / 100;
		SEG();
		if (BUZZER_STATE == 1) {
			y = 0;  //딩동댕 부저작동
			do {
				if(bu_state == 0){
					tone = song_1[y++]; // 노래 음계
				}
				if(bu_state != 0){
					tone = song[y++]; // 노래 음계
				}
				_delay_ms(100); // 한 계명당 지속 시간
			} while (tone != EOS); // 노래 마지막 음인지 검사
			BUZZER_STATE = 0;
		}
	}
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

ISR(INT4_vect) // sw1 interrupt 모드 변경
{
	state_value++;
	if(state_value == 3){
		state_value = 0;
	}

	if (state_value == 0) {
		value = 60;
		tick_time = 0;
		bu_state = 0;
	}
	if (state_value == 1) {
		value = 80;
		tick_time = 0;
		bu_state = 0;
	}
	if (state_value == 2) {
		value = 120;
		tick_time = 0;
		bu_state = 0;
	}
}

ISR(INT5_vect)  // sw2 interrupt 초기화
{
	tick_time = 0;
	bu_state = 0;
}

ISR(TIMER0_OVF_vect)  // timer interrupt
{
	TCNT0 = 256 - 125;
	tick_time++;
	if (tick_time == (240000 / value)) {
		BUZZER_STATE = 1;
		bu_state++;
		if(bu_state == 4){
			bu_state = 0;
		}
		tick_time = 0;
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
