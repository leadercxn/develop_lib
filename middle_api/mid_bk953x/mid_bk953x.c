#include "stdbool.h"

#ifdef F0
#include "f0_lib.h"
#endif

#include "board_config.h"
#include "mid_bk953x.h"


#ifdef F0

/* VIRT */
static int f0_virt_i2c_bk953x_read_one_reg(f0_virt_i2c_t *p_f0_virt_i2c, uint8_t device_id, uint8_t reg, uint32_t *p_data)
{
    uint8_t temp = 0;
    uint8_t *p = (uint8_t *)p_data;
    int err = 0;

    temp = (reg << 1) | 0x01;

    f0_virt_i2c_start(p_f0_virt_i2c);

    f0_virt_i2c_send_byte(p_f0_virt_i2c, device_id);

    err = f0_virt_i2c_wait_ack(p_f0_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    f0_virt_i2c_send_byte(p_f0_virt_i2c, temp);

    err = f0_virt_i2c_wait_ack(p_f0_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    //MSB
    p[3] = f0_virt_i2c_read_byte(p_f0_virt_i2c, true);
    p[2] = f0_virt_i2c_read_byte(p_f0_virt_i2c, true);
    p[1] = f0_virt_i2c_read_byte(p_f0_virt_i2c, true);
    p[0] = f0_virt_i2c_read_byte(p_f0_virt_i2c, false);

    f0_virt_i2c_stop(p_f0_virt_i2c);

    return err;
}

int f0_virt_i2c_bk953x_write_one_reg(f0_virt_i2c_t *p_f0_virt_i2c, uint8_t device_id,uint8_t reg,uint32_t *p_data)
{
    uint8_t temp = 0;
    uint8_t *p = (uint8_t *)p_data;
    int err = 0;
    int i = 0;

    temp = (reg << 1) & 0xfe;

    f0_virt_i2c_start(p_f0_virt_i2c);

    f0_virt_i2c_send_byte(p_f0_virt_i2c, device_id);
    err = f0_virt_i2c_wait_ack(p_f0_virt_i2c);
    if(err)
    {
        trace_error("slaver no ack!\n\r");
        return err;
    }

    f0_virt_i2c_send_byte(p_f0_virt_i2c, temp);

    err = f0_virt_i2c_wait_ack(p_f0_virt_i2c);
    if(err)
    {
        return err;
    }

    for(i = sizeof(uint32_t); i > 0; i--)
    {
        f0_virt_i2c_send_byte(p_f0_virt_i2c, p[i -1]);
        err = f0_virt_i2c_wait_ack(p_f0_virt_i2c);
        if(err)
        {
            trace_error("slaver no ack!\n\r");
            return err;
        }
    }

    f0_virt_i2c_stop(p_f0_virt_i2c);

    return err;
}


#endif


int mid_bk953x_read_one_reg(mid_bk953x_t *p_mid_bk953x, uint8_t device_id, uint8_t reg, uint32_t *p_data)
{
    int err = 0;

#ifdef F0
    err = f0_virt_i2c_bk953x_read_one_reg(&p_mid_bk953x->virt_i2c_object, device_id, reg, p_data);
#endif

    return err;
}

int mid_bk953x_write_one_reg(mid_bk953x_t *p_mid_bk953x, uint8_t device_id, uint8_t reg, uint32_t *p_data)
{
    int err = 0;
#ifdef F0
    err = f0_virt_i2c_bk953x_write_one_reg(&p_mid_bk953x->virt_i2c_object, device_id, reg, p_data);
#endif
    return err;
}


void mid_bk953x_res_init(mid_bk953x_t *p_mid_bk953x)
{
#ifdef F0
    f0_virt_i2c_init(&p_mid_bk953x->virt_i2c_object);
#endif

}

