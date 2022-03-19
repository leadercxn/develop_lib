#ifndef __MID_BK953X_H
#define __MID_BK953X_H

#ifdef F0
#include "f0_lib.h"
#endif

typedef enum
{
    I2C_TYPE_I2C1,
    I2C_TYPE_I2C2,
    I2C_TYPE_I2C3,
    I2C_TYPE_I2C4,
    I2C_TYPE_VIRT1,
    I2C_TYPE_VIRT2,
} i2c_type_e;

typedef struct 
{
    i2c_type_e      i2c_type;
#ifdef F0
    f0_virt_i2c_t   virt_i2c_object;
#endif

} mid_bk953x_t;


void mid_bk953x_res_init(mid_bk953x_t *p_mid_bk953x);
int mid_bk953x_read_one_reg(mid_bk953x_t *p_mid_bk953x, uint8_t device_id, uint8_t reg, uint32_t *p_data);
int mid_bk953x_write_one_reg(mid_bk953x_t *p_mid_bk953x, uint8_t device_id, uint8_t reg, uint32_t *p_data);

#endif
