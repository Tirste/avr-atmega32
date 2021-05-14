#define F_CPU 4000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	 //wyzerowanie przerwania
 cli();
 //konfiguracja diod
   DDRA = 0; //wyzerowanie 
   DDRC =0;
   DDRD = 0;
   DDRD |=(1<<PD2); //rezystor pullup
   DDRA |= (1<<PD0); //rezystor pullup
   PORTD|=(1<<PD2);
   PORTA = (1<<PA0); 
   PORTC = 0; //wyzerowanie wartosici portu c

   //konfiguracja przerwania
	MCUCR |= 1<<ISC01; 
    GICR |= 1<<INT0;
    //konfiguracja timera
	TIMSK = (1<<OCIE1A); //ustawienie przerwan dla komparatora
    TCCR1B |= (1<<WGM12); //tryb CTC
    TCCR1B |= 3 ;//PRESCALER 64  
	 sei();

//w³¹czenie linijki diod na porcie C (zolte diody)
    PORTC=0xFF;
	while(1) {
	_delay_ms(50);
	PORTC^=0xFF;
	}
}


ISR(INT0_vect) // przerwanie dla przycisku
{
	sei();
	if((PORTA & 0b00000001) == 0 )
	{
	OCR1A -= 5000; //odejmowanie o 0.08s

	if(OCR1A<TCNT1 || OCR1A<12000) TCNT1=OCR1A;

	PORTA |= (1<<PA7); //zaswiecenie diody podczas dzialania przycisku
	}
}

ISR(TIMER1_COMPA_vect) // przerwanie timera
{
	PORTA^=0b00000001;
	if(OCR1A == 20000) OCR1A=10000; OCR1A=20000; 
	PORTA&=0b01111111;
	sei();
}
