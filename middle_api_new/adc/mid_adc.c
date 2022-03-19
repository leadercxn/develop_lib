#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef F0
#include "f0_lib.h"
#endif

#include "board_config.h"
#include "mid_adc.h"

//ADC初始化宏
#ifdef F0 
#define ADC_INIT()   \
{                    \
    f0_adc_init();   \
}
#else

#define ADC_INIT()

#endif



void adc_init(void)
{
    ADC_INIT();
}

uint16_t adc_ch_result_get(uint32_t channel)
{
#ifdef FT32
    return adc_ch_value_get(channel);
#endif
}
