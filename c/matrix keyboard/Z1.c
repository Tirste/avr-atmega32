//WYKONA£ JAROS£AW KRÓLIKOWSKI
// 27.03.2021
// GR 4.1/2

#define F_CPU 4000000L	
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void konfiguracja() {

 /* KONFIGURACJA WE/WY */
   
   //wejscia
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
 
	//stan pocz¹tkowy portów - wyzerowanie
	PORTA = 0; //wyjscia
	PORTB = 0;
	PORTC = 0;
 
	//konfiguracja przyciskow
    DDRD |= 0xF0; //PD4-PD7 - wyjœcie
	PORTD &= 0x0F; //zerowanie linii
    
 /* KONFIGURACJA PRZERWANIA */
    DDRD &= ~(0<<PD2); //PD2 - 16 nozka procesora sluzaca do wywolania przerwania INT0
	PORTD |= (1<<PD2);	 //rezystor pull-up
    MCUCR |= (1<<ISC01); 							//INT0 - wyzwalanie zboczem opadaj¹cym
    GICR  |= (1<<INT0);								//w³¹czenie przerwania INT0
}



  int main(void)
  {

    cli(); //zerowanie flagi przerwañ w pamiêci
	konfiguracja(); //void z konfiguracj¹ 
    sei(); //w³aczenie przerwañ
 

    //POCZATEK ANIMACJI NA PORTC
   	PORTC = 0b11111111; 
    	while (1) 
    	{
		_delay_ms(50); //spowolnienie o 50ms
		PORTC ^= 0b11111111; //negacja bitowa portu C
    	}
   }
 



ISR(INT0_vect)
{
	PORTB = PORTB + 1;	//wyœwietlanie który raz przerwanie, po wciœniêciu klawisza w postaci binarnej
 
	for(int i=0; i<4; i++)	//sprawdzanie klawiszy
	{
		PORTD = (PORTD | 0xF0) & ~(1<<(4+i));	//zwarte linie wyjœciowych zerem
		if(~PIND & (1<<PD2))					//PD2 = 0, to wciœniêto zwarty klawisz
		PORTA = (1<<i);							//ustawienie PORTA
	}
 
	PORTD &= 0x0F; //zerowanie linii zwartych
	GIFR |= (0<<INT0); //czyszczenie flagi przerwañ
}
 

