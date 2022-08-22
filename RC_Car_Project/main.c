#include "common.h"
#include "uart.h"
#include "rf.h"

receiver receiver1;

uint16_t t1, t2;
uint16_t term;

uint8_t falling_cnt = 0;

void fast_pwm_init(void)
{
    cbi(SREG, 7);

    sbi(TCCR3A, COM1A1);
    cbi(TCCR3A, COM1A0);    // OC3A for DC Motor(Fast-PWM, non-inverting mode)
                            //If one or both of the COM1A1:0 bits are set, the
                            // OC3A output overrides the normal port functionality of the I/O pin it is connected to.

    sbi(TCCR4A, COM4C1);
    cbi(TCCR4A, COM4C0);    // OC4C for Servo Motor(Fast-PWM, non-inverting mode)
                            //If one or both of the COM4C1:0 bits are set, the
                            // OC4C output overrides the normal port functionality of the I/O pin it is connected to.

    cbi(TCCR3B, (CS02 | CS00));
    sbi(TCCR3B, CS01);

    cbi(TCCR4B, (CS02 | CS00));
    sbi(TCCR4B, CS01);  //prescaler : 8 -> clock source frequecy : 16/ 8 = 2MHz

    sbi(TCCR3A, WGM31);
    cbi(TCCR3A, WGM30);
    sbi(TCCR3B, (WGM33 | WGM32));   // OC3x Fast-PWM Mode
    ICR3 = 39999;    // period : 20ms frequency : 50Hz

    sbi(TCCR4A, WGM41);
    cbi(TCCR4A, WGM40);
    sbi(TCCR4B, (WGM43 | WGM42));   // OC4x Fast-PWM Mode
    ICR4 = 39999;    // period : 20ms frequency : 50Hz

    sbi(DDRE, DDE3);
    //sbi(DDRH, DDH5);    // DDR bit corresponding to the OC0A pin must be set in order to enable the output driver.

    sbi(SREG, 7);
}

int main(void)
{
    FILE *fd = fdevopen(usart_tx_char, NULL);

    uart_init();
    printf("UART Init Success!\n\r\0");
    rf_ppm_timer_set();
    printf("RF PPM Timer Set Success!\n\r\0");
    rf_receiver_init();
    printf("RF Receiver Init Success!\n\r\0");
    fast_pwm_init();
    printf("Fast PWM Init Success!\n\r\0");

    while(1)
    {
        printf("ch1: %d, ch2: %d, ch3: %d, ch4: %d, ch5: %d, ch6: %d, ch7: %d, ch8: %d, remainder: %d\n\r\0",\
                receiver1.ch1, receiver1.ch2, receiver1.ch3, receiver1.ch4,\
                receiver1.ch5, receiver1.ch6, receiver1.ch7, receiver1.ch8, receiver1.remainder_value);
        _delay_ms(500);
    }

    return 0;
}

ISR (INT4_vect)
{
    //printf("%d\r\n\0", falling_cnt);
    if(falling_cnt == 0)
    {
        t1 = TCNT1;
        //printf("hi\r\n\0");
        falling_cnt++;
    }
    else
    {
        t2 = TCNT1;
        //printf("bye\r\n\0");
        //falling_cnt++;
        // 시간 계산 (us)
        if (t1 > t2)
        {
            term = 39999 - t1 + t2;
        }
        else
        {
            term = t2 - t1;
        }
    }
    //printf("%d\r\n\0", falling_cnt);

    if (term > 10000)    // 해석 후 펄스폭이 5ms보다 크면
    {
        falling_cnt = 9;
    }

    if(term > 4000)
        term = 4000;
    else if(term < 2000)
        term = 2000;



    switch (falling_cnt)
    {
        // Use Transmitter : FS-i6 Receiver : FS-iA6B
        case 1: // Right throttle left-right
            receiver1.ch1 = term;
            t1 = t2;
            falling_cnt++;
            break;
        case 2: // Right throttle up-down
            receiver1.ch2 = term;
            t1 = t2;
            if(term > 3100)
            {
                sbi(PORTH, PORTH3);
                cbi(PORTH, PORTH4); // IN1 : 1 IN2 : 0 -> Forward
                OCR3A = (term - 3000) * 40000 / 1000;
                // If the OCRnx is set equal to BOTTOM (0x0000) the output will be a narrow spike for each
                // TOP+1 timer clock cycle. Setting the OCRnx equal to TOP will result in a constant high or low output (depending
                // on the polarity of the output set by the COMnx1:0 bits).
            }
            else if(term < 2900)
            {
                cbi(PORTH, PORTH3);
                sbi(PORTH, PORTH4); // IN1 : 0 IN2 : 1 -> Backward
                OCR3A = (3000 - term) * 40000 / 1000;
            }
            else
            {
                cbi(PORTH, PORTH3);
                cbi(PORTH, PORTH4); // IN1 : 0 IN2 :  -> Fast Stop
                OCR3A = 0;
            }
            falling_cnt++;
            break;
        case 3: // Left throttle up-down
            receiver1.ch3 = term;
            t1 = t2;
            falling_cnt++;
            break;
        case 4: // Left throttle left-right
            receiver1.ch4 = term;
            t1 = t2;
            OCR4C = term;   // Servo Motor PWM Duty Ratio : 5 ~ 10%
            falling_cnt++;
            break;
        case 5: // SWA
            receiver1.ch5 = term;
            t1 = t2;
            falling_cnt++;
            break;
        case 6: //SWC
            receiver1.ch6 = term;
            t1 = t2;
            falling_cnt++;
            break;
        case 7:
            receiver1.ch7 = term;
            t1 = t2;
            falling_cnt++;
            break;
        case 8:
            receiver1.ch8 = term;
            t1 = t2;
            falling_cnt++;
            break;
        case 9:
            receiver1.remainder_value = term;
            t1 = t2;
            falling_cnt = 1;
            break;
    }
}
