#define ON_OFF_VALUE 818
#include <avr/io.h>
#include <util/delay.h>

void init_adc();
unsigned short read_adc();
void show_7seg(unsigned short value);
unsigned char num[10] = { // 0 ~ 9
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};
int main() {
unsigned short value;
DDRC = 0xFF;
DDRG = 0x0F;
DDRA = 0xFF;
init_adc();
while(1) {
value = read_adc();
if( value < ON_OFF_VALUE)
PORTA = 0xFF;
else
PORTA = 0x00;

show_7seg(value);
}
return 0;
}
void init_adc()
{
DDRF &= 0xFE;
ADMUX = 0x40;
ADCSR = 0x87;
}
unsigned short read_adc()
{
unsigned char adc_low, adc_high;
unsigned short value;
ADCSR |= 0x40;
while ((ADCSR & 0x10) != 0x10)
;
adc_low = ADCL;
adc_high = ADCH;
value = ((unsigned short)adc_high << 8) | (unsigned short)adc_low;
return value;
}
void show_7seg(unsigned short value)
{
int i;
for(i=0; i<250; i++) {
PORTC = num[value/1000];
PORTG = 0x08;
_delay_ms(1);
PORTC = num[value/100%10];
PORTG = 0x04;
_delay_ms(1);
PORTC = num[value%100/10];
PORTG = 0x02;..
_delay_ms(1);
PORTC = num[value%10];
PORTG = 0x01;
_delay_ms(1);
}
}


