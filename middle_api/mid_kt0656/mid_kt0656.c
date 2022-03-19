#include "stdbool.h"

#include "board_config.h"
#include "mid_kt0656.h"


#ifdef HK32

/* VIRT */
static int hk_virt_i2c_kt0656_read_one_reg(hk_virt_i2c_t *p_virt_i2c, uint8_t device_addr, uint16_t reg, uint8_t *p_data)
{
    int err = 0;
    uint8_t addr_temp = 0;
    uint8_t temp = 0;

    addr_temp = (device_addr << 1) & 0xfe;

    hk_virt_i2c_start(p_virt_i2c);

    hk_virt_i2c_send_byte(p_virt_i2c, addr_temp);

    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    temp = (reg >> 8) & 0xff;
    hk_virt_i2c_send_byte(p_virt_i2c, temp);

    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    temp = reg & 0xff;
    hk_virt_i2c_send_byte(p_virt_i2c, temp);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    addr_temp = addr_temp | 0x01;

    hk_virt_i2c_start(p_virt_i2c);
    hk_virt_i2c_send_byte(p_virt_i2c, addr_temp);

    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    *p_data = hk_virt_i2c_read_byte(p_virt_i2c, false);

    hk_virt_i2c_stop(p_virt_i2c);

    return err;
}

int hk_virt_i2c_kt0656_write_one_reg(hk_virt_i2c_t *p_virt_i2c, uint8_t device_addr, uint16_t reg, uint8_t *p_data)
{
    int err = 0;
    uint8_t addr_temp = 0;
    uint8_t temp = 0;

    addr_temp = (device_addr << 1) & 0xfe;

    hk_virt_i2c_start(p_virt_i2c);

    hk_virt_i2c_send_byte(p_virt_i2c, addr_temp);

    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    temp = (reg >> 8) & 0xff;
    hk_virt_i2c_send_byte(p_virt_i2c, temp);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    temp = reg & 0xff;
    hk_virt_i2c_send_byte(p_virt_i2c, temp);
    err = hk_virt_i2c_wait_ack(p_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    hk_virt_i2c_send_byte(p_virt_i2c, *p_data);
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


int  mid_kt0656_read_one_reg(mid_kt0656_t *p_mid_kt0656, uint8_t device_addr, uint16_t reg, uint8_t *p_data)
{
    int err = 0;

#ifdef HK32
    err = hk_virt_i2c_kt0656_read_one_reg(&p_mid_kt0656->virt_i2c_object, device_addr, reg, p_data);
#endif

    return err;
}

int  mid_kt0656_write_one_reg(mid_kt0656_t *p_mid_kt0656, uint8_t device_addr, uint16_t reg, uint8_t *p_data)
{
    int err = 0;
#ifdef HK32
    err = hk_virt_i2c_kt0656_write_one_reg(&p_mid_kt0656->virt_i2c_object, device_addr, reg, p_data);
#endif
    return err;
}


void mid_kt0656_res_init(mid_kt0656_t *p_mid_kt0656)
{
#ifdef HK32
    hk_virt_i2c_init(&p_mid_kt0656->virt_i2c_object);
#endif

}

