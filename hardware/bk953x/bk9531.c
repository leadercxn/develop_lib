/**
 * 该文件是 BK9531 芯片配置相关寄存器接口 
 */

#include "board_config.h"
#include "bk9531.h"


#define MID_BK953X_READ(reg,p_data)     \
{                                       \
    err_code = mid_bk953x_read_one_reg(&p_bk953x_object->mid_bk953x_object, BK953X_DEVICE_ID, reg, p_data);   \
    if(err_code)                        \
    {                                   \
        trace_error("mid_bk953x_read reg 0x%02x error %d\n\r", reg ,err_code);                         \
        return err_code;               \
    }                                   \
}\
            

#define MID_BK953X_WRITE(reg,p_data)    \
{                                       \
    err_code = mid_bk953x_write_one_reg(&p_bk953x_object->mid_bk953x_object, BK953X_DEVICE_ID, reg, p_data);   \
    if(err_code)                        \
    {                                   \
        trace_error("mid_bk953x_write reg 0x%02x  error %d\n\r",reg ,err_code);                         \
        return err_code;               \
    }                                   \
}\

/**
 * V2.4 配置
 */
bk953x_reg_value_t g_bk9531_init_config[] = {
    {0x00 , 0x1C440C88},
    {0x01 , 0x04CF0057},
    {0x02 , 0x8990E02F},
//    {0x03 , 0x341206FF},    //UBAND,
    {0x03 , 0x341236FF},    //for cxn test U段,6分频
    {0x04 , 0x53880044},
    {0x05 , 0x00280380},
    {0x06 , 0x5BEDFB00},
    {0x07 , 0x1C400000},
    {0x08 , 0x00080100},
    {0x09 , 0x00007EFF},
    {0x0A , 0x0F3A3030},
    {0x0B , 0x0006C3FF},
    {0x0C , 0x00000008},
//  {0x0D , 0x3A980000},
    {0x0D , 0x4D260000},    //for cxn test , 632MHz

    {0x30 , 0x28282828},
    {0x31 , 0xD0000028},
    {0x32 , 0x10060064},
    {0x33 , 0x48808D82},
    {0x34 , 0x0B021108},
    {0x35 , 0x70500080},
    {0x36 , 0x0F801E04},
    {0x37 , 0x00000000},
    {0x38 , 0x00000000},    //ID_code
    {0x39 , 0x03D7D5F7},    //Preamble
    {0x3A , 0xC0250074},
    {0x3B , 0x9525003A},
    {0x3C , 0x9525003B},
    {0x3D , 0x9525003C},
    {0x3E , 0x00F867C3},
    {0x3F , 0x800F0000},

    {0x70 , 0x00009531},
    {0x71 , 0x18A40810},
    {0x72 , 0x00000000},
    {0x77 , 0x00070051},
    {0x78 , 0x00000008},
};

/**
 * V1.4 pdf配置
 */
bk953x_reg_value_t g_bk9531_init_config_v1_4[] = {
    {0x00, 0x1E440C88},
    {0x01, 0x04CF0057},
    {0x02, 0x8990E02F},
    {0x03, 0x341206FF},
    {0x04, 0x53880044},
    {0x05, 0x00280380},
    {0x06, 0x5BEDF800},
    {0x07, 0x1C400000},
    {0x08, 0x00080100},
    {0x09, 0x00007EFF},
    {0x0A, 0xCF2A4040},
    {0x0B, 0x0006C3FF},
    {0x0C, 0x00000008},
//  {0x0D, 0x3A980000},    //BK官方寄存器配置
    {0x0D, 0x4D260000},    //for cxn test , 632MHz

    {0x30, 0x28282828},
    {0x31, 0xD0000028},
    {0x32, 0x10060064},
    {0x33, 0x48808D82},
    {0x34, 0x0B021108},
    {0x35, 0x70500080},
    {0x36, 0x0F801E04},
    {0x37, 0x00000000},
    {0x38, 0x00000000},    //ID_code
    {0x39, 0x03D7D5F7},    //Preamble
    {0x3A, 0xC0250074},
    {0x3B, 0x9525003A},
    {0x3C, 0x9525003B},
    {0x3D, 0x9525003C},
    {0x3E, 0x00F867C3},
    {0x3F, 0x800F0000},

    {0x70, 0x00009531},
    {0x71, 0x18A40810},
    {0x72, 0x00000000},
    {0x77, 0x00070051},
    {0x78, 0x00000008},
};

int bk9531_tx_trigger(bk953x_object_t *p_bk953x_object)
{
    int err_code = 0;
    uint32_t value = 0;

    IS_NULL(p_bk953x_object);

    uint32_t reg_val_bak = g_bk9531_init_config[0x0A].value;

    g_bk9531_init_config[0x0A].value &= 0xFFFFFF00;
    MID_BK953X_WRITE(0x0A, &g_bk9531_init_config[0x0A].value);

    //Set ddf_en=0 for tx_trigger
    MID_BK953X_READ(0x35, &value);
    CLR_BIT(value,7);
    MID_BK953X_WRITE(0x35, &value);

    //Set tx_en=0 for tx_trigger
    MID_BK953X_READ(0x3F, &value);
    CLR_BIT(value,31);
    MID_BK953X_WRITE(0x3F, &value);

    //Set tx_en=0 another tx enable, from hds
    MID_BK953X_READ(0x39, &value);
    CLR_BIT(value,24);
    MID_BK953X_WRITE(0x39, &value);


    //Enable calibration clock
    SET_BIT(g_bk9531_init_config[7].value, 25);
    CLR_BIT(g_bk9531_init_config[7].value, 28);
    MID_BK953X_WRITE(0x07, &g_bk9531_init_config[7].value);

    //Calibrate RF VCO
    CLR_BIT(g_bk9531_init_config[3].value, 22);
    MID_BK953X_WRITE(0x03, &g_bk9531_init_config[3].value);
    delay_ms(2);

    SET_BIT(g_bk9531_init_config[3].value, 22);
    MID_BK953X_WRITE(0x03, &g_bk9531_init_config[3].value);
    delay_ms(2);
    
    CLR_BIT(g_bk9531_init_config[3].value, 22);
    MID_BK953X_WRITE(0x03, &g_bk9531_init_config[3].value);
    delay_ms(60);

    //Calibrate Digital VCO
    CLR_BIT(g_bk9531_init_config[4].value, 25);
    MID_BK953X_WRITE(0x04, &g_bk9531_init_config[4].value);
    SET_BIT(g_bk9531_init_config[4].value, 25);
    MID_BK953X_WRITE(0x04, &g_bk9531_init_config[4].value);

    //Disable calibration clock
    CLR_BIT(g_bk9531_init_config[7].value, 25);
    SET_BIT(g_bk9531_init_config[7].value, 28);
    MID_BK953X_WRITE(0x07, &g_bk9531_init_config[7].value);

    //Set ddf_en=1 for tx_trigger
    MID_BK953X_READ(0x35, &value);
    SET_BIT(value,7);
    MID_BK953X_WRITE(0x35, &value);

    //Set tx_en=1 for tx_trigger
    MID_BK953X_READ(0x3F, &value);
    SET_BIT(value,31);
    MID_BK953X_WRITE(0x3F, &value);

    //Set tx_en=1 another tx enable, from hds
    MID_BK953X_READ(0x39, &value);
    SET_BIT(value,24);
    MID_BK953X_WRITE(0x39, &value);

    MID_BK953X_WRITE(0x0A, &reg_val_bak);

    return err_code;
}

int bk9531_tx_freq_chan_set(bk953x_object_t *p_bk953x_object, freq_chan_object_t *p_freq_chan_object)
{
    IS_NULL(p_bk953x_object);
    IS_NULL(p_freq_chan_object);

    p_bk953x_object->band_type = p_freq_chan_object->band_type;
    p_bk953x_object->freq_chan_index = p_freq_chan_object->chan_index;

    int err_code = 0;
    uint32_t value = 0;

    /**
     * 6分频
     */
    CLR_BIT(g_bk9531_init_config[3].value, 15);
    CLR_BIT(g_bk9531_init_config[3].value, 14);
    SET_BIT(g_bk9531_init_config[3].value, 13);

    /**
     * U段
     */
    CLR_BIT(g_bk9531_init_config[3].value, 21);
    SET_BIT(g_bk9531_init_config[3].value, 20);
    MID_BK953X_WRITE(0x03, &g_bk9531_init_config[3].value);

    if(p_freq_chan_object)
    {
        value = p_freq_chan_object->reg_value;
    }
    else
    {
        value = 0x4D260000; //632MHz
    }

    MID_BK953X_WRITE(0x0D, &value);

    bk9531_tx_trigger(p_bk953x_object);

    return err_code;
}

int bk9531_tx_spec_data_set(bk953x_object_t *p_bk953x_object , uint8_t data)
{
    int err_code = 0;
    uint32_t value = 0;

    //Set ddf_en=0 for tx_trigger
    MID_BK953X_READ(0x3A, &value);
    value &= 0xFFFFFF00;

    value |= data;
    MID_BK953X_WRITE(0x3A, &value);

    return err_code;
}

int bk9531_tx_id_set(bk953x_object_t *p_bk953x_object , uint32_t id)
{
    int err_code = 0;
    uint32_t tx_id = id;

    MID_BK953X_WRITE(0x38, &tx_id);

    return err_code;
}

int bk9531_tx_mic_rssi_get(bk953x_object_t *p_bk953x_object, uint8_t *p_data)
{
    int err_code = 0;
    uint32_t value = 0;

    IS_NULL(p_bk953x_object);
    IS_NULL(p_data);

    MID_BK953X_READ(0x77, &value);

    *p_data = (value & 0xFF) - 0x3f;

    return err_code;
} 

int bk9531_config_init(bk953x_object_t *p_bk953x_object)
{
    int err_code = 0;
    uint8_t i = 0;

    for(i = 0 ; i < ARRAY_SIZE(g_bk9531_init_config) ; i ++)
    {
        err_code = mid_bk953x_write_one_reg(&p_bk953x_object->mid_bk953x_object, BK953X_DEVICE_ID, g_bk9531_init_config[i].reg, &g_bk9531_init_config[i].value);
        if(err_code)
        {
            trace_warn("bk953x_config_init  wite reg 0x%02x fail\n\r",g_bk9531_init_config[i].reg);
        }
    }

/**
 * 软复位
 */


/**
 * 先粗暴的配置
 */
#if 1
    trace_debug("write all reg done\n\r");
//    bk9531_reg_printf(p_bk953x_object);

    bk9531_tx_trigger(p_bk953x_object);

    trace_debug("bk9531_tx_trigger done\n\r");
    bk9531_reg_printf(p_bk953x_object);
#endif
    trace_debug("bk9531_config_init  done\n\r");

    return err_code;
}

int bk9531_chip_id_get(bk953x_object_t *p_bk953x_object)
{
    uint32_t value = 0;
    uint8_t rty = 0;

    int err_code = 0;

    /* 重试 5 次 */
    for(rty = 0; rty < 5; rty++)
    {
        err_code = mid_bk953x_read_one_reg(&p_bk953x_object->mid_bk953x_object, BK953X_DEVICE_ID, REG_CHIP_ID, &value);
        if(err_code == 0)
        {
            p_bk953x_object->chip_id = value;
            break;
        }

        delay_ms(50);
    }

    trace_debug("bk953x_chip_id_get err_code = %d , chip_id = 0x%08x\n\r",err_code,p_bk953x_object->chip_id);
    return err_code;
}

void bk9531_res_init(bk953x_object_t *p_bk953x_object)
{
    mid_bk953x_res_init(&p_bk953x_object->mid_bk953x_object);
}

int bk9531_reg_printf(bk953x_object_t *p_bk953x_object)
{
    int err_code = 0;

    uint8_t i = 0;
    uint8_t reg = 0;
    uint32_t reg_value = 0;

    for(i = 0; i < (ARRAY_SIZE(g_bk9531_init_config)); i++)
    {
        reg = g_bk9531_init_config[i].reg;

        MID_BK953X_READ(reg,&reg_value);

        trace_debug("bk9531 reg = 0x%02x , reg_value = 0x%08x\n\r",reg, reg_value);
    }

    return 0;
}

int bk9531_reg_read(bk953x_object_t *p_bk953x_object, uint8_t reg, uint32_t *p_data)
{
    int err_code = 0;

    MID_BK953X_READ(reg, p_data);

    return err_code;
}

int bk9531_reg_write(bk953x_object_t *p_bk953x_object, uint8_t reg, uint32_t *p_data)
{
    int err_code = 0;

    MID_BK953X_WRITE(reg, p_data);

    return err_code;
}
