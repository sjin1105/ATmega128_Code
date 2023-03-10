#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>



int main()
{
	unsigned char FND_SEGNP[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};
	unsigned char FND_SEGWP[10]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0xA7,0xFF,0xEF};
	
	DDRC = 0xFF;
	DDRG |= 0x0F;

	DDRB = DDRB | 0x10;


	volatile unsigned long int i1;
	volatile unsigned long int count = 0;
	
	while(1)
	{
		if((PINE & 0x20) == 0x00){
		count++;
		_delay_ms(8);
		}

		PORTC = FND_SEGNP[count%10];
		PORTG = 0x01;
		_delay_ms(1);

		PORTC = FND_SEGNP[count%60/10%10];
		PORTG = 0x02;
		_delay_ms(1);

		PORTC = FND_SEGWP[count/60*100/100%10];
		PORTG = 0x04;
		_delay_ms(1);

		PORTC = FND_SEGNP[count/60*100/1000%10];
		PORTG = 0x08;
		_delay_ms(1);

		if((PINE & 0x10) == 0x00)
		{
			while(1)
			{
				for(count = count; count > 0; count--)
				{
					for(i1=0; i1<10; i1++)
					{
						PORTC = FND_SEGNP[count%10];
						PORTG = 0x01;
						_delay_ms(1);

						PORTC = FND_SEGNP[count%60/10%10];
						PORTG = 0x02;
						_delay_ms(1);

						PORTC = FND_SEGWP[count/60*100/100%10];
						PORTG = 0x04;
						_delay_ms(1);

						PORTC = FND_SEGNP[count/60*100/1000%10];
						PORTG = 0x08;
						_delay_ms(1);
					}

				}
				if(count == 0)
				break;
			}
	
		}
	}

return 0;
}
