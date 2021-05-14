//WYKONA£ JAROS£AW KRÓLIKOWSKI
// 10.04.2021
// GR 4.1/2


#define F_CPU 4000000L  //konfiguracja predkosci zegara
#include <avr/io.h>
#include <avr/interrupt.h> //biblioteka na przerwanie
#include <avr/delay.h> //biblioteka na opoznienie
volatile int cnst, sign;    //zmienne pomocnicze

char tab[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; //tablica z danymi


void komenda(unsigned char var )
{
	PORTA = (PORTA & 0x0F) | (var & 0xF0); // wysylanie wysokiego stanu
	PORTA &= ~ (1<<0);		//wysylanie komendy, RS=0
	PORTA |= (1<<1);		// wlaczenie
	_delay_ms(1);
	PORTA &= ~ (1<<1);
	_delay_ms(20);
	PORTA = (PORTA & 0x0F) | (var << 4);  // wysylanie niskiego stanu
	PORTA |= (1<<1);
	_delay_us(1);
	PORTA &= ~ (1<<1);
	_delay_ms(2);
}


void dane( unsigned char data )
{
	PORTA = (PORTA & 0x0F) | (data & 0xF0); // wysylanie wysokiego stanu
	PORTA |= (1<<0);		//rejestr danych, RS=1 
	PORTA|= (1<<1);
	_delay_ms(1);
	PORTA &= ~ (1<<1);
	_delay_ms(20);
	PORTA = (PORTA & 0x0F) | (data << 4); // wysylanie niskiego stanu
    PORTA |= (1<<1);
	_delay_ms(1);
	PORTA &= ~ (1<<1);
	_delay_ms(2);
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

void drukuj (char *string)		// przyjecie znaku do drukujenia
{
		dane(string[sign]);
		cnst++;
		_delay_ms(50); //delay dodany w celu wyelminowania sytuacji wielokrotnego wcisniecia przycisku,
		// wiem, ze nie mozna stosowac delay w timerze
}
void konfiguracja_timera()
{
//tu skonfigurowalem timer i ustawilem odpowiednie flagi
    TCCR0 |= (1<<WGM01);    // ustawilem tryb na  CTC
    TCCR0 |= (1<<CS01 |CS00);        //ustawilem prescaler clk/64
    OCR0 = 125;        //wartosc z kalkulatora         
    TIMSK |= (1<<OCIE0);    //ustawienie flagi na wlaczenie przewania od cs
}

void konfiguracja_przerwan() {
    DDRB |= 0xF0;     
    DDRB &= 0xF0;    
    PORTB |= 0x0F;    
	DDRD=1;
	PORTD=(1<<PD2);	
	DDRD |=(1<<PD2); //rezystor pullup	
	MCUCR |= 1<<ISC01; //zbocze narastajace
    GICR |= 1<<INT0; //wlaczenie przerwan na INT0
}


int main(void)
{
	cli(); //wy³¹czenie przerwañ globalnych
	konfiguracja_przerwan(); //konfiguracja przerwwan
    konfiguracja(); // funkcja ustawienia portow i ddrd
    konfiguracja_timera(); //funkcja ustawienia timera
    sei(); //zezwolenie na przerwania 
    while(1);
}


ISR(TIMER0_COMP_vect) // przerwanie po timerze
{     
    for(int i=0; i<4; i++) { //petla sprawdzajaca wiersze multipleksera
		if(cnst<16) {
			sign = i;
        	PORTB = (PORTB| 0xF0) & ~(1<<(4+i));    //ustawienie poczatkowe PORTD 
        	if(~PINB & (1<<PB0)){drukuj(tab);}
       		if(~PINB & (1<<PB1)) {sign+=4;drukuj(tab);}// sprawdzenie drugiego wiersza 
            if(~PINB & (1<<PB2)) {sign+=8;drukuj(tab);}// sprawdzenie trzeciego wiersza 
            if(~PINB & (1<<PB3)) {sign+=12;drukuj(tab);}// sprawdzenie czwartego  wiersza czy wcisniety byl ktorys z klawiszy i  w³ozenie odpowiedniej wartoci 'i' do PORTA zeby zaswiecilo binarnie diody odpowiadajace przyciskowi
           }                      
      }
 
 PORTD &= 0x0F; //zerowanie linii zwartych 

}

