//Poni�ej podaj swoje dane
//Andrzej Nieznany
//11111
//IINS4.11
//GL09
//Cw1Z1

/* pojedyncza dioda animowana (PB0) w main; puszczenie klawisza(INT0)
prze��cza diod� PD7 */




///////////////////////
#define F_CPU 4000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void)
{
    cli(); //wy��czenie przerwa�

    DDRD = DDRD | 0b10000000; //
//    DDRB | = 0b10000000;
    DDRA = DDRA | 0b00000001;  //

    //konfiguracja wej�cia INT0
    DDRD = DDRD & ~(1<<PD2); //wyzerowanie bitu na PIND2 -> wej�cie
    PORTD |= (1<<PD2); //rezystor pull-up na PIND2

    MCUCR |= (1<<ISC01)|(1<<ISC00); //INT0 - zbocze narastaj�ce
    GICR |= (1<<INT0);    //w��czenie przerwania z INT0
    sei(); //globalne zezwolenie na przerwania

    while (1) 
    {

    }
}


ISR(INT0_vect)
{
while(1)
{
_delay_ms(50);
    PORTA= PORTA ^ 0b00000001;
    }
}
