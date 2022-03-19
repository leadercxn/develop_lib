#include "stdbool.h"

#include "board_config.h"
#include "mid_kt0646.h"


#ifdef HK32

/* VIRT */
static int hk_virt_i2c_kt0646_read_one_reg(hk_virt_i2c_t *p_virt_i2c, uint8_t device_addr, uint8_t reg, uint16_t *p_data)
{
    int err = 0;
    uint8_t addr_temp = 0;
    uint8_t *p = (uint8_t *)p_data;

    addr_temp = (device_addr << 1) & 0xfe;

    hk_virt_i2c_start(p_virt_i2c);

    hk_virt_i2c_send_byte(p_virt_i2c, addr_temp);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    hk_virt_i2c_send_byte(p_virt_i2c, reg);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    //start
    hk_virt_i2c_start(p_virt_i2c);

    addr_temp = addr_temp | 0x01;
    hk_virt_i2c_send_byte(p_virt_i2c, addr_temp);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    p[1] = hk_virt_i2c_read_byte(p_virt_i2c, true);
    p[0] = hk_virt_i2c_read_byte(p_virt_i2c, false);

    hk_virt_i2c_stop(p_virt_i2c);

    return err;
}

int hk_virt_i2c_kt0646_write_one_reg(hk_virt_i2c_t *p_virt_i2c, uint8_t device_addr, uint8_t reg, uint16_t *p_data)
{
    int err = 0;
    uint8_t addr_temp = 0;
    uint8_t *p = (uint8_t *)p_data;

    addr_temp = (device_addr << 1) & 0xfe;

    hk_virt_i2c_start(p_virt_i2c);

    hk_virt_i2c_send_byte(p_virt_i2c, addr_temp);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    hk_virt_i2c_send_byte(p_virt_i2c, reg);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    hk_virt_i2c_send_byte(p_virt_i2c, p[1]);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    hk_virt_i2c_send_byte(p_virt_i2c, p[0]);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    hk_virt_i2c_stop(p_virt_i2c);
    return err;
}


#endif


int  mid_kt0646_read_one_reg(mid_kt0646_t *p_mid_kt0646, uint8_t device_addr, uint8_t reg, uint16_t *p_data)
{
    int err = 0;

#ifdef HK32
    err = hk_virt_i2c_kt0646_read_one_reg(&p_mid_kt0646->virt_i2c_object, device_addr, reg, p_data);
#endif

    return err;
}

int  mid_kt0646_write_one_reg(mid_kt0646_t *p_mid_kt0646, uint8_t device_addr, uint8_t reg, uint16_t *p_data)
{
    int err = 0;
#ifdef HK32
    err = hk_virt_i2c_kt0646_write_one_reg(&p_mid_kt0646->virt_i2c_object, device_addr, reg, p_data);
#endif
    return err;
}


void mid_kt0646_res_init(mid_kt0646_t *p_mid_kt0646)
{
#ifdef HK32
    hk_virt_i2c_init(&p_mid_kt0646->virt_i2c_object);
#endif

}

