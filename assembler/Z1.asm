.include "m32def.inc"

    .org 0x00
    jmp main

    .org 0x100
; g³ówna czesc progamu
main:
    sbi porta ,0 ;port ALED0
    sbi ddra ,0  ;port A0 jako wyjscie

;petla
loop:

    sbic pina ,1     ;pomija jezeli nie wcisjnie pliku
    sbi porta ,0    ; zapala diode
    sbis pina ,1    ;jezeli klawisz wcisniety to pomija
    cbi porta,0     ;wylacza diode
    rjmp loop ;konie petli
