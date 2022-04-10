#ifndef _f1_SYSTICK_H_
#define _f1_SYSTICK_H_

#include "stdbool.h"

#ifdef HK32F103
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#endif

void SysTick_Init(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

void f1_systick_init(void);
void f1_delay_us(uint32_t us);
void f1_delay_ms(uint32_t ms);

uint32_t f1_get_systick(void);

#endif
