#ifndef __F0_LIB_H
#define __F0_LIB_H

#ifdef FT32
#include "ft32f0xx.h"
#endif 

#ifdef HK32
#include "hk32f0xx.h"
#endif 

#include "f0_gpio.h"
#include "f0_i2c.h"
//#include "f0_delay.h"
#include "f0_timer.h"
#include "f0_usart.h"
#include "f0_timer_pwm.h"
#include "f0_exit.h"
#include "f0_adc.h"
#include "f0_flash.h"
#include "f0_systick.h"


#endif
