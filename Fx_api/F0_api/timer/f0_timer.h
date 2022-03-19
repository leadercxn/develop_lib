#ifndef __F0_TIMER_H
#define __F0_TIMER_H

typedef void (*timer_handler_t) (void);

void timer3_init(void);

void timer_handler_register(timer_handler_t handler);

uint64_t f0_timer_tick_get(void);

#endif

