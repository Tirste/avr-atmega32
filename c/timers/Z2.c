#define F_CPU 4000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



int main(void) {
    DDRD = (1 << PD5); // WYJSCIE OC1A
    TCCR1A = (1 << WGM11) | (1 << COM1A1); //TRYB COMPARE OUT
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1<<CS11); // KONFIGURACJA TRYBU DLA MODE 14(FASTPWM)
    while(1) {                                  //PRESKALER CLK - 8
        ICR1 = 28499; //CALY PRZEBIEG 19+38MS
		OCR1A = 9499; //NARASTAJACY 19MS 
        _delay_ms(200); // OPOZNIENIE ABY WIDZIEC ZMIANY W STANIE PIND(5)
    }
}
