#include "board_config.h"
#include "mid_systick.h"


void mid_system_tick_init(void)
{
#ifdef FT32
    ft_systick_init();
#endif

#ifdef HK32
    hk_systick_init();
#endif

}



