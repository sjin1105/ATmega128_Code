#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char FND_SEGNP[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};  // 7SEG
unsigned char FND_SEGWP[10]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0xA7,0xFF,0xEF};  // 7SEG + (.)
unsigned char FND_SEGPOS[4]={0x01,0x02,0x04,0x08};

 int main()
{
	DDRC = 0xFF;
	DDRG |= 0x0F;

	DDRE = DDRE & 0xCF;

	DDRB = DDRB | 0x10;

 	PORTC = 0xFF;
 	PORTG = 0x0F;

	int a=0,b=0,c=0,d=0,count=0;
	int j,i;

	EXIT:

	for(i=0;i<1000;++i){
	PORTB = PORTB ^ 0x10;
	_delay_ms(10);
	}

	a=0,b=0,c=0,d=0,count=0;

while(1){
	
	if((PINE & 0x20) == 0x00){
		count++;
		if(count==10)
		count = 0;
		PORTG = FND_SEGPOS[2];
		PORTC = FND_SEGWP[count];
		_delay_ms(100);
		}
	
if((PINE & 0x10) == 0x00){
while(1)
{
	for (j=0; j<4; j++)
	{
	PORTG = FND_SEGPOS[j];

		if(j==0){
			if(a==10){
			a=0;
			b++;
			}
			PORTC = FND_SEGNP[a];
			a++;
			_delay_ms(10);
			}

		if(j==1){
			if(b==10){
			b=0;
			c++;
			}
			PORTC = FND_SEGNP[b];
			_delay_ms(10);
			}

		if(j==2){
			if(c==10){
			c=0;
			d++;
			}
			if(c==count)
			goto EXIT;
			PORTC = FND_SEGWP[c];
			_delay_ms(10);
			}

		if(j==3){
			if(d==6){
			d=0;
			}
			PORTC = FND_SEGNP[d];
			_delay_ms(10);
			}
		}
	

	}
	}
	}
return 0;
}
