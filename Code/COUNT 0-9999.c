#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>



int main()
{
	unsigned char FND_SEGNP[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};
	unsigned char FND_SEGWP[10]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0xA7,0xFF,0xEF};
	unsigned char FND_SEGPOS[4]={0x01,0x02,0x04,0x08};
	
	DDRC = 0xFF;
	DDRG |= 0x0F;


	volatile unsigned long int i1,i2;
	
	while(1)
	{
		for(i2 = 0; i2 < 9999; i2++)
		{
			for(i1=0; i1<50; i1++)
			{
				PORTC = FND_SEGNP[i2%10];
				PORTG = 0x01;
				_delay_ms(1);

				PORTC = FND_SEGNP[i2/10%10];
				PORTG = 0x02;
				_delay_ms(1);

				PORTC = FND_SEGNP[i2/100%10];
				PORTG = 0x04;
				_delay_ms(1);

				PORTC = FND_SEGNP[i2/1000];
				PORTG = 0x08;
				_delay_ms(1);
			}
		}
	}

return 0;
}
