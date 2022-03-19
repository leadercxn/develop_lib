#ifndef __MID_KT0646_H
#define __MID_KT0646_H


typedef struct 
{
    i2c_type_e      i2c_type;
#ifdef  F0
    f0_virt_i2c_t   virt_i2c_object;
#endif

} mid_kt0646_t;


void mid_kt0646_res_init(mid_kt0646_t *p_mid_kt0646);
int  mid_kt0646_read_one_reg(mid_kt0646_t *p_mid_kt0646, uint8_t device_addr, uint8_t reg, uint16_t *p_data);
int  mid_kt0646_write_one_reg(mid_kt0646_t *p_mid_kt0646, uint8_t device_addr, uint8_t reg, uint16_t *p_data);

#endif
