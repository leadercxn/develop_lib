#ifdef F0
#include "f0_lib.h"
#endif

#include "board_config.h"
#include "mid_delay.h"

void delay_us(uint16_t us)
{
#ifdef F0
    f0_delay_us(us);
#elif defined F1
    f1_delay_us(us);
#endif
}

void delay_ms(uint16_t ms)
{
#ifdef F0
    f0_delay_ms(ms);
#elif defined F1
    f1_delay_ms(ms);
#endif
}
