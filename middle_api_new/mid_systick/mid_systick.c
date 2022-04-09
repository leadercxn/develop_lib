#include "board_config.h"
#include "mid_systick.h"


void mid_system_tick_init(void)
{
#ifdef F0
    f0_systick_init();
#elif defined F1
    f1_systick_init();
#endif
}



