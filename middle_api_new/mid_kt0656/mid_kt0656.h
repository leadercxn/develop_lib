#ifndef __MID_KT0656_H
#define __MID_KT0656_H

typedef struct 
{
    i2c_type_e      i2c_type;
#ifdef  F0
    f0_virt_i2c_t   virt_i2c_object;
#endif

} mid_kt0656_t;


void mid_kt0656_res_init(mid_kt0656_t *p_mid_kt0656);
int  mid_kt0656_read_one_reg(mid_kt0656_t *p_mid_kt0656, uint8_t device_addr, uint16_t reg, uint8_t *p_data);
int  mid_kt0656_write_one_reg(mid_kt0656_t *p_mid_kt0656, uint8_t device_addr, uint16_t reg, uint8_t *p_data);

#endif
