#include <avr/io.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수에 변수 가능
#include <util/delay.h>

double num[7] = {1047, 1175, 1319, 1397, 1568, 1760, 1976};

int main()
{
	DDRB = DDRB | 0x10;

	volatile unsigned long int i,j;

	while(1){

	for(j = 0; j < 7; j++){
		for(i = 0; i < num[j] * 2; i++){
			_delay_ms(1000 / num[j]);
			PORTB = PORTB ^ 0x10;
			}
		}
	}

	return 0;
}
