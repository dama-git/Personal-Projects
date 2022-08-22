#include "common.h"
#include "rf.h"

void rf_receiver_init(void)
{
    cbi(SREG, 7);   // Global Interrupt Disable

    cbi(DDRB, PE4); // PE4 Pin Input
    cbi(PORTB, PE4);    // PE4 Pin Pull-Up Resistor Disable

    sbi(EICRB, ISC41);
    cbi(EICRB, ISC40);   // Falling Edge generates an interrupt request

    sbi(EIMSK, INT4); // INT4 Pin contributes to the Interrupt
    sbi(EIFR, INTF4);  // INT4 Flag Clear

    sbi(SREG, 7);   //Global Interrupt Enable
}

void rf_ppm_timer_set(void)
{
    cbi(SREG, 7);   // Global Interrupt Disable

    cbi(TCCR1A, WGM10);
    cbi(TCCR1A, WGM11);
    sbi(TCCR1B, WGM12);
    cbi(TCCR1B, WGM13); // CTC Mode

    sbi(TCCR1B, CS11);  // prescaler : 8 -> 16 / 8 = 2MHz Clock Source, 1 tick : 0.5us

    OCR1A = 39999; // frequency : 2MHz / 40000 = 50Hz period : 20ms

    sbi(SREG, 7);   //Global Interrupt Enable

}

