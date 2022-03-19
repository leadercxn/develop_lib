#ifndef __F0_DELAY_H
#define __F0_DELAY_H

#ifdef FT32
#include "ft32f0xx.h"
#endif 

#ifdef HK32F0
#include "hk32f0xx.h"
#endif

void f0_delay_10us(uint16_t n_10us);
void f0_delay_ms(uint16_t ms);

#endif

