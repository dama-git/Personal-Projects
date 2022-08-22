#ifndef __RF_H__
#define __RF_H__

#include    <stdint.h>

typedef struct
{
    uint16_t    ch1;
    uint16_t    ch2;
    uint16_t    ch3;
    uint16_t    ch4;
    uint16_t    ch5;
    uint16_t    ch6;
    uint16_t    ch7;
    uint16_t    ch8;

    uint16_t    remainder_value;
}receiver;

void rf_receiver_init(void);
void rf_ppm_timer_set(void);
#endif // __RF_H__

