#ifndef _f0_SYSTICK_H_
#define _f0_SYSTICK_H_

#include "stdbool.h"

#ifdef FT32
#include "ft32f0xx.h"
#endif 

#ifdef HK32F0
#include "hk32f0xx.h"
#endif

void SysTick_Init(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

void f0_systick_init(void);
void f0_delay_us(uint32_t us);
void f0_delay_ms(uint32_t ms);

#endif
