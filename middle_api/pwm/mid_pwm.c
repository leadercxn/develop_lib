#include "stdbool.h"

#ifdef F0
#include "f0_lib.h"
#endif


#include "mid_pwm.h"


void pwm_init(mid_pwm_object *p_mid_pwm_object)
{
#ifdef F0
    timer15_pwm_ch1_init(p_mid_pwm_object->freq, p_mid_pwm_object->duty_cycle);
#endif

}

