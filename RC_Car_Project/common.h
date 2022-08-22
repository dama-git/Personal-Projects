#ifndef __COMMON_H__
#define __CPMMON_H__

#define F_CPU   16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define sbi(PORTx, BITx)    (PORTx |= (1 << BITx))
#define cbi(PORTx, BITx)    (PORTx &= ~(1 << BITx))

#endif // __COMMON_H__

