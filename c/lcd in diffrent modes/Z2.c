//WYKONA£ JAROS£AW KRÓLIKOWSKI
// 10.04.2021
// GR 4.1/2


#define F_CPU 4000000L	//czestotliwosc zegara na 4MHz
#include <avr/io.h>			
#include <util/delay.h>	// biblioteka na opoznienie
#include <avr/interrupt.h> //biblioteka na przerwania

volatile int sign; //zmienna pomocnicza 
char tab[]={'J','a','r','o','s','l','a','w',0}; //wyswietlane imie znak po znaku
 

void komenda( unsigned char var )
{
	PORTA = (PORTA & 0x0F) | (var & 0xF0); // wysylanie wysokiego stanu
	PORTA &= ~ (1<<PB0);	//wysylanie komendy, RS=0
	PORTA |= (1<<PB1);		
	_delay_us(10); //opoznienie 10us
	PORTA &= ~ (1<<PB1); 
 
	_delay_us(10); //opoznienie 10us
 
	PORTA = (PORTA & 0x0F) | (var << 4);  //wysylanie niskiego stanu
	PORTA |= (1<<PB1);
	_delay_us(1);
	PORTA &= ~ (1<<PB1);
	_delay_ms(20);
}

void konfiguracja (void)  
{
	DDRA = 0xFF;		//uruchomienie wyswietlacza
	_delay_ms(20);		// opoznienie zwiazane z odswiezaniem wystwietlacza
	komenda(0x33);
	komenda(0x32);	//4 znaki inicjalizacji
	komenda(0x28);	// tryb 4-bit, 5 x 7 kropek
	komenda(0x0F);	// kursor w trybie migania
	komenda(0x06);	// kursor od lewej do prawej
	komenda(0x01);	// czyszczenie wyswietlacza
} 

void konfiguracja_przerwan(){
    DDRD = 1; //ustawienie rezystora pull-up
    PORTD = (1<<PD2); //port 1 skonfigurowany do przerwan
 	MCUCR |= (1<<ISC01)|(1<<ISC00); 
    MCUCR |= (1<<ISC11)|(1<<ISC10);
    GICR |= (1<<INT0); 
    GICR |= (1<<INT1); 
}
 
 
void dane( unsigned char data )
{
	PORTA = (PORTA & 0x0F) | (data & 0xF0); //wyslanie wysokiego stanu
	PORTA |= (1<<PB0);		//rejestr danych, RS=1 
	PORTA|= (1<<PB1);
	_delay_us(1);
	PORTA &= ~ (1<<PB1);
 
	_delay_us(200);
 
	PORTA = (PORTA & 0x0F) | (data << 4); //wysylanie niskiego stanu
	PORTA |= (1<<PB1);
	_delay_us(1);
	PORTA &= ~ (1<<PB1);
	_delay_ms(2);
}
 
void drukuj(char *string) //uzyj stringa w pamieci i wykorzystaj jako wskaŸnik
{
		dane (string[sign]);
}
 

int main()
{
	konfiguracja_przerwan(); /
	konfiguracja();		
    sei();
	while(1) {};
}
 
 
ISR(INT0_vect) // przerwanie ISR0 - przycisk next
{
	if(tab[sign]!=0) {
	drukuj(tab);
	sign++;
	}

	else {
	GICR |= (0<<INT0); 
	}
		
}
 
ISR(INT1_vect) //przerwanie ISR1 - przycisk clear
{
	komenda(0x01);
 	sign = 0;
}
