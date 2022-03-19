#ifndef __BK9531_H
#define __BK9531_H

typedef struct
{
    uint8_t  reg;
    uint32_t value;
} bk953x_reg_value_t;

typedef enum
{
    BK953X_BAND_V,  //V段
    BK953X_BAND_U   //U段
} bk953x_band_type_e;

typedef enum
{
    SA_ANT1_PIN6 = 0,                   //使用PIN6作为天线输入口    2020-05-26
    SA_ANT2_PIN5,                       //使用PIN5作为天线输入口    2020-05-26
    DA_AUTO,                            //自动天线模式
} antenna_type_e;

typedef struct
{
    bk953x_band_type_e  band_type;
    uint16_t            chan_index;
    uint32_t            freq;
    uint32_t            reg_value;
} freq_chan_object_t;

typedef void (*bk953x_hw_reset_handler) (void);

typedef struct 
{
    uint16_t                    chip_id;
    uint8_t                     device_id;
    bk953x_band_type_e          band_type;
    antenna_type_e              antenna_type;
    uint16_t                    freq_chan_index;
    mid_bk953x_t                mid_bk953x_object;

    void                        *p_rst_gpio;    //指向复位引脚
} bk953x_object_t;

#define BK9531_CHID_ID      0x9531

#define REG_CHIP_ID 0x70

int bk9531_tx_trigger(bk953x_object_t *p_bk953x_object);

int bk9531_tx_freq_chan_set(bk953x_object_t *p_bk953x_object, freq_chan_object_t *p_freq_chan_object);

int bk9531_tx_id_set(bk953x_object_t *p_bk953x_object , uint32_t id);

int bk9531_tx_spec_data_set(bk953x_object_t *p_bk953x_object , uint8_t data);

int bk9531_tx_mic_rssi_get(bk953x_object_t *p_bk953x_object, uint8_t *p_data);

int bk9531_config_init(bk953x_object_t *p_bk953x_object);

int bk9531_chip_id_get(bk953x_object_t *p_bk953x_object);

void bk9531_res_init(bk953x_object_t *p_bk953x_object);

int bk9531_reg_printf(bk953x_object_t *p_bk953x_object);

int bk9531_reg_read(bk953x_object_t *p_bk953x_object, uint8_t reg, uint32_t *p_data);

int bk9531_reg_write(bk953x_object_t *p_bk953x_object, uint8_t reg, uint32_t *p_data);

#endif
