//JAROSLAW KROLIKOWSKI 094233

#define F_CPU 4000000UL

#include <util/delay.h>
#include <avr/interrupt.h> //do³¹czenie biblioteki z przerwaniami
#include <avr/io.h>

uint8_t key_lock1;

int main(void)
{
cli();
DDRA = 0;
DDRC = 0;
DDRD |=(1<<PD2);
PORTD|=(1<<PD2); 
PORTD|=(1<<PD4);
PORTA |=(0b00001111);

MCUCR |= (0<<ISC00)|(1<<ISC01); //konfiguracja przerwania INT0
GICR |= 1<<INT0; //w³¹czenie przerwania INT0
       
	 while(1){
PORTC=0b00000000;
_delay_ms(20) ; 
PORTC = 0b11111111; 
_delay_ms(20) ;
sei();
} //globalne w³¹czenie przerwañ
	  while(1) //pusta pêtla
    {
   	}
}

ISR(INT0_vect) //pocz¹tek funkcji obs³ugi przerwania
{
PORTA ^=(0b00001111);
PORTA ^=~(0b00001111);
}

