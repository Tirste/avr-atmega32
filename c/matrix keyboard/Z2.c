//WYKONA£ JAROS£AW KRÓLIKOWSKI
// 27.03.2021
// GR 4.1/2

#define F_CPU 4000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

volatile int count; //wskazuje na liniê 


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


ISR(TIMER0_COMP_vect)
{	//petla realizujaca zliczanie 
	PORTD &= ~(1<<(4+count));	//wystawienie zera na liniê strobuj¹c¹
	if(~PIND & 0x0F) // je¿eli linie wierszy to 1111, pomiñ
	{
		PORTA = (0<<(4+count));	//flaga aktywnej kolumny
		PORTA = ~PIND; //flaga aktywnego wiersza
	}

	PORTD |= (1<<(4+count));	//wystawienie bitu 1 na zwart¹ linie

	if(count == 4) count = 0; //je¿eli licznik==4 to zwiera od nowa
	else count++; //jezeli wiekszy to dodaje
}
