#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/io.h> 
#include <avr/interrupt.h> 

unsigned char FND_SEGNP[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};
unsigned char FND_SEGWP[10]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0xA7,0xFF,0xEF};
unsigned char FND_SEGPOS[4]={0x01,0x02,0x04,0x08};

volatile unsigned int i1, i2, count, sec;

int main()
{
	DDRC = 0xFF;
	DDRG |= 0x0F;

	EICRA = 0x00;
	EICRB = 0x0A;
	EIMSK = 0x30;
	sei();
 
	i2 = 0x00; 
	
	i1 = 0;

	while(1)
	{

		count++;		
		sec = count % 250;

		PORTC = FND_SEGNP[i2%10];
		PORTG = 0x01;
		_delay_ms(1);

		PORTC = FND_SEGNP[i2%60/10%10];
		PORTG = 0x02;
		_delay_ms(1);

		PORTC = FND_SEGWP[i2/60*100/100%10];
		PORTG = 0x04;
		_delay_ms(1);

		PORTC = FND_SEGNP[i2/60*100/1000%10];
		PORTG = 0x08;
		_delay_ms(1);
		if(i1 == 1){

			if(sec == 0){
			i2--;
			}

			if(i2 == 0){
			i1 = 0;
			}
		}
	}

	return (1);
}

ISR(INT4_vect) 
{
	i2++;
	_delay_ms(100);
}

ISR(INT5_vect)
{
	i1= 1;
}
