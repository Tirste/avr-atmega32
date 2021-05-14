//WYKONA£ JAROS£AW KRÓLIKOWSKI
// 27.03.2021
// GR 4.1/2

#define F_CPU 4000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>


void konf_timer() {

    TCCR0 |= (1<<WGM01); //tryb CTC
	TCCR0 |= (1<<CS00) | (1<<CS01);		//prescaler 64
	OCR0 =  123;		//timer odlicza ok. 0.5s tiki od 122 do 255
	TIMSK |= (1<<OCIE0);	//w³¹czenie przerwania COMP

}

void konf_o_i() {
DDRD |= 0xF0;	//PD4-7 jako wyjœcia
	DDRD &= 0xF0;	//PD0-3 jako linie wejœciowe
	PORTD |= 0x0F;	//rezystory pull-up dla wierszy
	DDRA = 0xFF;//	//PORTA jako wyjscie dla linijki LED
	DDRC = 0xFF;	//PORTC jako wyjscie
}

int main(void)
{
    cli(); //wy³¹czenie przerwañ globalnych
	konf_o_i();
	konf_timer();
	sei();

	//animacja na linijce C
	PORTC = 0x00; 
    while (1) 
    {	_delay_ms(50); //opoznienie 50ms
		PORTC ^= 0xFF;	//negacja bitowa
    }	

}

int x= 4;

ISR(TIMER0_COMP_vect)
{    //petla realizujaca zliczanie 
    for(int i=0; i<x; i++)	//sprawdzanie klawiszy
	{
		
		PORTD = (PORTD | 0xF0) & ~(1<<(x+i));	//zwarty wiersz
		
		if(~PIND & (1<<PD0)) {PORTA = i;} //zwarta kolumna 1
			
			if(~PIND & (1<<PD1)) {PORTA = i+x;} ///zwarta kolumna 2
				
				if(~PIND & (1<<PD2)) {PORTA = i+(x*2);} ///zwarta kolumna 3
					
					if(~PIND & (1<<PD3)) {PORTA = i+(x*3);} ///zwarta kolumna 4					
	}
 
  
	PORTD &= 0x0F; //zerowanie linii zwartych 

}
