//WYKONA£ JAROS£AW KRÓLIKOWSKI
// 10.04.2021
// GR 4.1/2


#define F_CPU 4000000UL //czestotliwosc zegara na 4MHz
#include <avr/io.h> 
#include <util/delay.h> //biblioteka na opoznienie
 
void konfiguracja()
{
	_delay_ms(10); //opoznienie 10ms
	komenda(0x38); //tryb 2 linie, 5x8 kropek
	komenda(0x0E); //w³¹cza wyœwietlacz
	komenda(0x01); //czyszczenie wyœwietlacza
	_delay_ms(10); //opoznienie 10ms

}

void komenda (unsigned char var)
{
	PORTA  = var; //tu wplywaja dane
	PORTB &= ~(PB1<< 2) | ~(PB1<< 0);
	PORTB |= (PB1<< 1);
	_delay_us(2); //opoznienie 2us
	PORTB &= ~(PB1<< 1);
	_delay_us(10);
}

void dane (unsigned char data)
{
	PORTA  = data; //tu wplywaja dane
	PORTB &= ~(PB1<< 2); 
	PORTB |= (PB1<< 0) | (PB1<< 1);
	_delay_us(2);
	PORTB &= ~(PB1<< 1);
	_delay_us(100); //opoznienie 100us
}
 

void drukuj(char *string) //uzyj stringa w pamieci i wykorzystaj jako wskaŸnik
{
	while(*string > 0)
	{
		dane(*string++);		
	}
}


int main(void)
{
    DDRB = 0xFF; //rezystory pull-up
	PORTB = 0x00; //wyzerowanie portu
	DDRA = 0xFF; //rezystory pull-up
	PORTA  = 0x00; //wyzerowanie portu
	PORTA = (0x01); //czyszczenie LCD
    konfiguracja(); //konfiguracja wyswietlacza
	drukuj("Krolikowski"); //drukuj nazwisko
	
	while(1) {} 
}
