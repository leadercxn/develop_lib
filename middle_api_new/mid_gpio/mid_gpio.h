#ifndef __MID_GPIO_H
#define __MID_GPIO_H

#ifdef	 F0
#include "f0_lib.h"
#endif

#ifdef F1
#include "f1_lib.h"
#endif

typedef enum
{
    GPIO_DIR_OUTPUR,
    GPIO_DIR_INPUT,
} gpio_dir_e;

typedef enum
{
    MODE_IO  =   0,
    MODE_AF  =   1,
}gpio_mode_e;

typedef struct
{
#if (defined F0) || (defined F1)
    uint32_t        gpio_port_periph_clk;
    GPIO_TypeDef    *p_gpio_port;
#endif

    uint32_t    gpio_pin;
    gpio_dir_e  gpio_dir;
    uint32_t    flag;           // 用作是否复用的标志，0--无复用，1--有复用
} gpio_object_t;

int gpio_config(gpio_object_t *p_gpio_object);

int gpio_output_set(gpio_object_t *p_gpio_object, uint8_t value);

int gpio_input_get(gpio_object_t *p_gpio_object, uint8_t *p_value);
#endif
