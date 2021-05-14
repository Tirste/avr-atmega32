#define F_CPU 4000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void main(void)
{
 //wyzerowanie przerwania
 cli();
 //konfiguracja diod
 DDRA = 0; //zerowanie ddra
 DDRC =0;  //zerowanie ddrc
 DDRD |=(1<<PD2); //rezystor pullup
 DDRA = (1<<PD0); //rezystor pullup
 PORTD|=(1<<PD2); //uruchomienie przycisku pd2
 PORTA = (0<<PA0); //wylaczenie diody pa0
 //konfiguracaj przerwania
 MCUCR |= 1<<ISC01; //wlaczanie przerwania jako stan wysoki
 GICR |= 1<<INT0;  // wlaczanie przerwan INT0
 TIMSK |= (1<<TOIE1); //wlaczenie przerwan po przepelnieniu 
 TCCR1B |= (0<<WGM10) | (0<<WGM11) | (0<<WGM12) | (0<<WGM13);   //timer tryb normalny, wszystko wy³¹czone 
 sei();


//w³¹czenie linijki diod na porcie C (zolte diody)
 	PORTC=0xFF; //uruchomienie zoltej linijki diod
	while(1) {
	_delay_ms(50); //opoznienie o 50ms
	PORTC^=0xFF; //wylaczenie zoltej linijki diod
	}

}
 
 //przerwanie
 ISR(INT0_vect) {
 PORTA = (1<<PA0);
 TCNT1 =  31250: //takt zegara od 31250 do 2^16
 TCCR1B |= (1<CS11) | (1<<CS10); // preskaler 64
} 


//timer 16bitowy timer1
ISR(TIMER1_OVF_vect) {
   PORTA = (0<<PA0); //zgaszenie diody
   TCCR1B &= (0<<CS11) | (0<<CS10); //stop timer
}
