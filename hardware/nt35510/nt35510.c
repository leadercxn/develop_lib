#include "nt35510.h"
#include "board_config.h"


/**
 * @brief  nt35510发送命令函数
 * 
 * @param[i] p_dev      驱动信息结构体
 * @param[i] cmd        命令
 * 
 * @retval 
 */
int nt35510_write_cmd(lcd_drv_t *p_dev, uint16_t cmd)
{
    p_dev->lcd_addr->lcd_cmd = cmd;
    return 0;
}

/**
 * @brief  nt35510发送数据
 * 
 * @param[i] p_dev      驱动信息结构体
 * @param[i] data       下发的数据
 * 
 * @retval 
 */
int nt35510_write_data(lcd_drv_t *p_dev, uint16_t data)
{
    p_dev->lcd_addr->lcd_data = data;
    return 0;
}

/**
 * @brief  nt35510读数据
 * 
 * @param[i] p_dev      驱动信息结构体
 * 
 * @retval data         读对应地址的数据
 */
int nt35510_read_data(lcd_drv_t *p_dev)
{
    uint16_t data;
    data = p_dev->lcd_addr->lcd_data;
    return data;
}

/**
 * @brief  nt35510下发带参数命令
 * 
 * @param[i] p_dev      驱动信息结构体
 * @param[i] cmd        下发的命令
 * @param[i] data       命令参数
 * 
 * @retval data         读对应地址的数据
 */
int nt35510_write_reg(lcd_drv_t *p_dev, uint16_t cmd, uint16_t data)
{
    p_dev->lcd_addr->lcd_cmd = cmd;
    p_dev->lcd_addr->lcd_data = data;
    return 0;
}


/**
 * @brief  nt35510读取对应命令的返回值
 * 
 * @param[i] p_dev      驱动信息结构体
 * @param[i] cmd        下发的命令
 * 
 * @retval data         读命令回显值
 */
int nt35510_read_reg(lcd_drv_t *p_dev, uint16_t cmd)
{
    uint16_t data;
    p_dev->lcd_addr->lcd_cmd = cmd;
    delay_us(5);
    data = p_dev->lcd_addr->lcd_data;
    return data;
}


/**
 * @brief  nt35510初始化
 * 
 * @param[i] p_drv    驱动信息结构体      
 * 
 * @retval /
 */
int nt35510_init(driver_info_t *p_drv)
{
    IS_NULL(p_drv);
    lcd_drv_t *p_dev = p_drv->dev;

    // 1. gpio config
    gpio_config(&p_dev->ctrl_fsmc.rst_pin);
    gpio_config(&p_dev->ctrl_fsmc.back_light);
    gpio_config(&p_dev->ctrl_fsmc.fsmc_pin1);
    gpio_config(&p_dev->ctrl_fsmc.fsmc_pin2);

    // 2.fsmc config
	fsmc_conig(0, BUS_WIDTH_16b);

    // 3. reset
    set_gpio_value(GPIOE, GPIO_Pin_1, 0);
    delay_ms(1000);
    set_gpio_value(GPIOE, GPIO_Pin_1, 1);

    // 4. 设置lcd_addr
    p_dev->lcd_base_addr = ((uint32_t)((FSMC_BASE_ADDR + BANK1_SECTOR1_OFFSET) | BANK_16B_A16_OFFSET));
    p_dev->lcd_addr = ((LCD_Type_16b_t *)(p_dev->lcd_base_addr));

    // 5. init nt35510
    // 5.1 read lcd id
    p_drv->write_cmd(p_dev, 0XDA00);
    p_drv->lcd_param->id = p_drv->read_data(p_dev);     //读回0X00	 
    p_drv->write_cmd(p_dev, 0XDB00);
    p_drv->lcd_param->id = p_drv->read_data(p_dev);     //读回0X80
    p_drv->lcd_param->id <<= 8;
    p_drv->write_cmd(p_dev, 0XDC00);
    p_drv->lcd_param->id |= p_drv->read_data(p_dev);    //读回0X00
    if (p_drv->lcd_param->id == 0x8000)
    {
        p_drv->lcd_param->id = 0x5510;
    }
    else
    {
        return -EINVAL;
    }

    // 5.2 init sequence
    p_drv->write_reg(p_dev, 0xF000, 0x55);
    p_drv->write_reg(p_dev, 0xF001, 0xAA);
    p_drv->write_reg(p_dev, 0xF002, 0x52);
    p_drv->write_reg(p_dev, 0xF003, 0x08);
    p_drv->write_reg(p_dev, 0xF004, 0x01);
    //AVDD Set AVDD 5.2V
    p_drv->write_reg(p_dev, 0xB000, 0x0D);
    p_drv->write_reg(p_dev, 0xB001, 0x0D);
    p_drv->write_reg(p_dev, 0xB002, 0x0D);
    //AVDD ratio
    p_drv->write_reg(p_dev, 0xB600, 0x34);
    p_drv->write_reg(p_dev, 0xB601, 0x34);
    p_drv->write_reg(p_dev, 0xB602, 0x34);
    //AVEE -5.2V
    p_drv->write_reg(p_dev, 0xB100, 0x0D);
    p_drv->write_reg(p_dev, 0xB101, 0x0D);
    p_drv->write_reg(p_dev, 0xB102, 0x0D);
    //AVEE ratio
    p_drv->write_reg(p_dev, 0xB700, 0x34);
    p_drv->write_reg(p_dev, 0xB701, 0x34);
    p_drv->write_reg(p_dev, 0xB702, 0x34);
    //VCL -2.5V
    p_drv->write_reg(p_dev, 0xB200, 0x00);
    p_drv->write_reg(p_dev, 0xB201, 0x00);
    p_drv->write_reg(p_dev, 0xB202, 0x00);
    //VCL ratio
    p_drv->write_reg(p_dev, 0xB800, 0x24);
    p_drv->write_reg(p_dev, 0xB801, 0x24);
    p_drv->write_reg(p_dev, 0xB802, 0x24);
    //VGH 15V (Free pump)
    p_drv->write_reg(p_dev, 0xBF00, 0x01);
    p_drv->write_reg(p_dev, 0xB300, 0x0F);
    p_drv->write_reg(p_dev, 0xB301, 0x0F);
    p_drv->write_reg(p_dev, 0xB302, 0x0F);
    //VGH ratio
    p_drv->write_reg(p_dev, 0xB900, 0x34);
    p_drv->write_reg(p_dev, 0xB901, 0x34);
    p_drv->write_reg(p_dev, 0xB902, 0x34);
    //VGL_REG -10V
    p_drv->write_reg(p_dev, 0xB500, 0x08);
    p_drv->write_reg(p_dev, 0xB501, 0x08);
    p_drv->write_reg(p_dev, 0xB502, 0x08);
    p_drv->write_reg(p_dev, 0xC200, 0x03);
    //VGLX ratio
    p_drv->write_reg(p_dev, 0xBA00, 0x24);
    p_drv->write_reg(p_dev, 0xBA01, 0x24);
    p_drv->write_reg(p_dev, 0xBA02, 0x24);
    //VGMP/VGSP 4.5V/0V
    p_drv->write_reg(p_dev, 0xBC00, 0x00);
    p_drv->write_reg(p_dev, 0xBC01, 0x78);
    p_drv->write_reg(p_dev, 0xBC02, 0x00);
    //VGMN/VGSN -4.5V/0V
    p_drv->write_reg(p_dev, 0xBD00, 0x00);
    p_drv->write_reg(p_dev, 0xBD01, 0x78);
    p_drv->write_reg(p_dev, 0xBD02, 0x00);
    //VCOM
    p_drv->write_reg(p_dev, 0xBE00, 0x00);
    p_drv->write_reg(p_dev, 0xBE01, 0x64);
    //Gamma Setting
    p_drv->write_reg(p_dev, 0xD100, 0x00);
    p_drv->write_reg(p_dev, 0xD101, 0x33);
    p_drv->write_reg(p_dev, 0xD102, 0x00);
    p_drv->write_reg(p_dev, 0xD103, 0x34);
    p_drv->write_reg(p_dev, 0xD104, 0x00);
    p_drv->write_reg(p_dev, 0xD105, 0x3A);
    p_drv->write_reg(p_dev, 0xD106, 0x00);
    p_drv->write_reg(p_dev, 0xD107, 0x4A);
    p_drv->write_reg(p_dev, 0xD108, 0x00);
    p_drv->write_reg(p_dev, 0xD109, 0x5C);
    p_drv->write_reg(p_dev, 0xD10A, 0x00);
    p_drv->write_reg(p_dev, 0xD10B, 0x81);
    p_drv->write_reg(p_dev, 0xD10C, 0x00);
    p_drv->write_reg(p_dev, 0xD10D, 0xA6);
    p_drv->write_reg(p_dev, 0xD10E, 0x00);
    p_drv->write_reg(p_dev, 0xD10F, 0xE5);
    p_drv->write_reg(p_dev, 0xD110, 0x01);
    p_drv->write_reg(p_dev, 0xD111, 0x13);
    p_drv->write_reg(p_dev, 0xD112, 0x01);
    p_drv->write_reg(p_dev, 0xD113, 0x54);
    p_drv->write_reg(p_dev, 0xD114, 0x01);
    p_drv->write_reg(p_dev, 0xD115, 0x82);
    p_drv->write_reg(p_dev, 0xD116, 0x01);
    p_drv->write_reg(p_dev, 0xD117, 0xCA);
    p_drv->write_reg(p_dev, 0xD118, 0x02);
    p_drv->write_reg(p_dev, 0xD119, 0x00);
    p_drv->write_reg(p_dev, 0xD11A, 0x02);
    p_drv->write_reg(p_dev, 0xD11B, 0x01);
    p_drv->write_reg(p_dev, 0xD11C, 0x02);
    p_drv->write_reg(p_dev, 0xD11D, 0x34);
    p_drv->write_reg(p_dev, 0xD11E, 0x02);
    p_drv->write_reg(p_dev, 0xD11F, 0x67);
    p_drv->write_reg(p_dev, 0xD120, 0x02);
    p_drv->write_reg(p_dev, 0xD121, 0x84);
    p_drv->write_reg(p_dev, 0xD122, 0x02);
    p_drv->write_reg(p_dev, 0xD123, 0xA4);
    p_drv->write_reg(p_dev, 0xD124, 0x02);
    p_drv->write_reg(p_dev, 0xD125, 0xB7);
    p_drv->write_reg(p_dev, 0xD126, 0x02);
    p_drv->write_reg(p_dev, 0xD127, 0xCF);
    p_drv->write_reg(p_dev, 0xD128, 0x02);
    p_drv->write_reg(p_dev, 0xD129, 0xDE);
    p_drv->write_reg(p_dev, 0xD12A, 0x02);
    p_drv->write_reg(p_dev, 0xD12B, 0xF2);
    p_drv->write_reg(p_dev, 0xD12C, 0x02);
    p_drv->write_reg(p_dev, 0xD12D, 0xFE);
    p_drv->write_reg(p_dev, 0xD12E, 0x03);
    p_drv->write_reg(p_dev, 0xD12F, 0x10);
    p_drv->write_reg(p_dev, 0xD130, 0x03);
    p_drv->write_reg(p_dev, 0xD131, 0x33);
    p_drv->write_reg(p_dev, 0xD132, 0x03);
    p_drv->write_reg(p_dev, 0xD133, 0x6D);
    p_drv->write_reg(p_dev, 0xD200, 0x00);
    p_drv->write_reg(p_dev, 0xD201, 0x33);
    p_drv->write_reg(p_dev, 0xD202, 0x00);
    p_drv->write_reg(p_dev, 0xD203, 0x34);
    p_drv->write_reg(p_dev, 0xD204, 0x00);
    p_drv->write_reg(p_dev, 0xD205, 0x3A);
    p_drv->write_reg(p_dev, 0xD206, 0x00);
    p_drv->write_reg(p_dev, 0xD207, 0x4A);
    p_drv->write_reg(p_dev, 0xD208, 0x00);
    p_drv->write_reg(p_dev, 0xD209, 0x5C);
    p_drv->write_reg(p_dev, 0xD20A, 0x00);

    p_drv->write_reg(p_dev, 0xD20B, 0x81);
    p_drv->write_reg(p_dev, 0xD20C, 0x00);
    p_drv->write_reg(p_dev, 0xD20D, 0xA6);
    p_drv->write_reg(p_dev, 0xD20E, 0x00);
    p_drv->write_reg(p_dev, 0xD20F, 0xE5);
    p_drv->write_reg(p_dev, 0xD210, 0x01);
    p_drv->write_reg(p_dev, 0xD211, 0x13);
    p_drv->write_reg(p_dev, 0xD212, 0x01);
    p_drv->write_reg(p_dev, 0xD213, 0x54);
    p_drv->write_reg(p_dev, 0xD214, 0x01);
    p_drv->write_reg(p_dev, 0xD215, 0x82);
    p_drv->write_reg(p_dev, 0xD216, 0x01);
    p_drv->write_reg(p_dev, 0xD217, 0xCA);
    p_drv->write_reg(p_dev, 0xD218, 0x02);
    p_drv->write_reg(p_dev, 0xD219, 0x00);
    p_drv->write_reg(p_dev, 0xD21A, 0x02);
    p_drv->write_reg(p_dev, 0xD21B, 0x01);
    p_drv->write_reg(p_dev, 0xD21C, 0x02);
    p_drv->write_reg(p_dev, 0xD21D, 0x34);
    p_drv->write_reg(p_dev, 0xD21E, 0x02);
    p_drv->write_reg(p_dev, 0xD21F, 0x67);
    p_drv->write_reg(p_dev, 0xD220, 0x02);
    p_drv->write_reg(p_dev, 0xD221, 0x84);
    p_drv->write_reg(p_dev, 0xD222, 0x02);
    p_drv->write_reg(p_dev, 0xD223, 0xA4);
    p_drv->write_reg(p_dev, 0xD224, 0x02);
    p_drv->write_reg(p_dev, 0xD225, 0xB7);
    p_drv->write_reg(p_dev, 0xD226, 0x02);
    p_drv->write_reg(p_dev, 0xD227, 0xCF);
    p_drv->write_reg(p_dev, 0xD228, 0x02);
    p_drv->write_reg(p_dev, 0xD229, 0xDE);
    p_drv->write_reg(p_dev, 0xD22A, 0x02);
    p_drv->write_reg(p_dev, 0xD22B, 0xF2);
    p_drv->write_reg(p_dev, 0xD22C, 0x02);
    p_drv->write_reg(p_dev, 0xD22D, 0xFE);
    p_drv->write_reg(p_dev, 0xD22E, 0x03);
    p_drv->write_reg(p_dev, 0xD22F, 0x10);
    p_drv->write_reg(p_dev, 0xD230, 0x03);
    p_drv->write_reg(p_dev, 0xD231, 0x33);
    p_drv->write_reg(p_dev, 0xD232, 0x03);
    p_drv->write_reg(p_dev, 0xD233, 0x6D);
    p_drv->write_reg(p_dev, 0xD300, 0x00);
    p_drv->write_reg(p_dev, 0xD301, 0x33);
    p_drv->write_reg(p_dev, 0xD302, 0x00);
    p_drv->write_reg(p_dev, 0xD303, 0x34);
    p_drv->write_reg(p_dev, 0xD304, 0x00);
    p_drv->write_reg(p_dev, 0xD305, 0x3A);
    p_drv->write_reg(p_dev, 0xD306, 0x00);
    p_drv->write_reg(p_dev, 0xD307, 0x4A);
    p_drv->write_reg(p_dev, 0xD308, 0x00);
    p_drv->write_reg(p_dev, 0xD309, 0x5C);
    p_drv->write_reg(p_dev, 0xD30A, 0x00);

    p_drv->write_reg(p_dev, 0xD30B, 0x81);
    p_drv->write_reg(p_dev, 0xD30C, 0x00);
    p_drv->write_reg(p_dev, 0xD30D, 0xA6);
    p_drv->write_reg(p_dev, 0xD30E, 0x00);
    p_drv->write_reg(p_dev, 0xD30F, 0xE5);
    p_drv->write_reg(p_dev, 0xD310, 0x01);
    p_drv->write_reg(p_dev, 0xD311, 0x13);
    p_drv->write_reg(p_dev, 0xD312, 0x01);
    p_drv->write_reg(p_dev, 0xD313, 0x54);
    p_drv->write_reg(p_dev, 0xD314, 0x01);
    p_drv->write_reg(p_dev, 0xD315, 0x82);
    p_drv->write_reg(p_dev, 0xD316, 0x01);
    p_drv->write_reg(p_dev, 0xD317, 0xCA);
    p_drv->write_reg(p_dev, 0xD318, 0x02);
    p_drv->write_reg(p_dev, 0xD319, 0x00);
    p_drv->write_reg(p_dev, 0xD31A, 0x02);
    p_drv->write_reg(p_dev, 0xD31B, 0x01);
    p_drv->write_reg(p_dev, 0xD31C, 0x02);
    p_drv->write_reg(p_dev, 0xD31D, 0x34);
    p_drv->write_reg(p_dev, 0xD31E, 0x02);
    p_drv->write_reg(p_dev, 0xD31F, 0x67);
    p_drv->write_reg(p_dev, 0xD320, 0x02);
    p_drv->write_reg(p_dev, 0xD321, 0x84);
    p_drv->write_reg(p_dev, 0xD322, 0x02);
    p_drv->write_reg(p_dev, 0xD323, 0xA4);
    p_drv->write_reg(p_dev, 0xD324, 0x02);
    p_drv->write_reg(p_dev, 0xD325, 0xB7);
    p_drv->write_reg(p_dev, 0xD326, 0x02);
    p_drv->write_reg(p_dev, 0xD327, 0xCF);
    p_drv->write_reg(p_dev, 0xD328, 0x02);
    p_drv->write_reg(p_dev, 0xD329, 0xDE);
    p_drv->write_reg(p_dev, 0xD32A, 0x02);
    p_drv->write_reg(p_dev, 0xD32B, 0xF2);
    p_drv->write_reg(p_dev, 0xD32C, 0x02);
    p_drv->write_reg(p_dev, 0xD32D, 0xFE);
    p_drv->write_reg(p_dev, 0xD32E, 0x03);
    p_drv->write_reg(p_dev, 0xD32F, 0x10);
    p_drv->write_reg(p_dev, 0xD330, 0x03);
    p_drv->write_reg(p_dev, 0xD331, 0x33);
    p_drv->write_reg(p_dev, 0xD332, 0x03);
    p_drv->write_reg(p_dev, 0xD333, 0x6D);
    p_drv->write_reg(p_dev, 0xD400, 0x00);
    p_drv->write_reg(p_dev, 0xD401, 0x33);
    p_drv->write_reg(p_dev, 0xD402, 0x00);
    p_drv->write_reg(p_dev, 0xD403, 0x34);
    p_drv->write_reg(p_dev, 0xD404, 0x00);
    p_drv->write_reg(p_dev, 0xD405, 0x3A);
    p_drv->write_reg(p_dev, 0xD406, 0x00);
    p_drv->write_reg(p_dev, 0xD407, 0x4A);
    p_drv->write_reg(p_dev, 0xD408, 0x00);
    p_drv->write_reg(p_dev, 0xD409, 0x5C);
    p_drv->write_reg(p_dev, 0xD40A, 0x00);
    p_drv->write_reg(p_dev, 0xD40B, 0x81);

    p_drv->write_reg(p_dev, 0xD40C, 0x00);
    p_drv->write_reg(p_dev, 0xD40D, 0xA6);
    p_drv->write_reg(p_dev, 0xD40E, 0x00);
    p_drv->write_reg(p_dev, 0xD40F, 0xE5);
    p_drv->write_reg(p_dev, 0xD410, 0x01);
    p_drv->write_reg(p_dev, 0xD411, 0x13);
    p_drv->write_reg(p_dev, 0xD412, 0x01);
    p_drv->write_reg(p_dev, 0xD413, 0x54);
    p_drv->write_reg(p_dev, 0xD414, 0x01);
    p_drv->write_reg(p_dev, 0xD415, 0x82);
    p_drv->write_reg(p_dev, 0xD416, 0x01);
    p_drv->write_reg(p_dev, 0xD417, 0xCA);
    p_drv->write_reg(p_dev, 0xD418, 0x02);
    p_drv->write_reg(p_dev, 0xD419, 0x00);
    p_drv->write_reg(p_dev, 0xD41A, 0x02);
    p_drv->write_reg(p_dev, 0xD41B, 0x01);
    p_drv->write_reg(p_dev, 0xD41C, 0x02);
    p_drv->write_reg(p_dev, 0xD41D, 0x34);
    p_drv->write_reg(p_dev, 0xD41E, 0x02);
    p_drv->write_reg(p_dev, 0xD41F, 0x67);
    p_drv->write_reg(p_dev, 0xD420, 0x02);
    p_drv->write_reg(p_dev, 0xD421, 0x84);
    p_drv->write_reg(p_dev, 0xD422, 0x02);
    p_drv->write_reg(p_dev, 0xD423, 0xA4);
    p_drv->write_reg(p_dev, 0xD424, 0x02);
    p_drv->write_reg(p_dev, 0xD425, 0xB7);
    p_drv->write_reg(p_dev, 0xD426, 0x02);
    p_drv->write_reg(p_dev, 0xD427, 0xCF);
    p_drv->write_reg(p_dev, 0xD428, 0x02);
    p_drv->write_reg(p_dev, 0xD429, 0xDE);
    p_drv->write_reg(p_dev, 0xD42A, 0x02);
    p_drv->write_reg(p_dev, 0xD42B, 0xF2);
    p_drv->write_reg(p_dev, 0xD42C, 0x02);
    p_drv->write_reg(p_dev, 0xD42D, 0xFE);
    p_drv->write_reg(p_dev, 0xD42E, 0x03);
    p_drv->write_reg(p_dev, 0xD42F, 0x10);
    p_drv->write_reg(p_dev, 0xD430, 0x03);
    p_drv->write_reg(p_dev, 0xD431, 0x33);
    p_drv->write_reg(p_dev, 0xD432, 0x03);
    p_drv->write_reg(p_dev, 0xD433, 0x6D);
    p_drv->write_reg(p_dev, 0xD500, 0x00);
    p_drv->write_reg(p_dev, 0xD501, 0x33);
    p_drv->write_reg(p_dev, 0xD502, 0x00);
    p_drv->write_reg(p_dev, 0xD503, 0x34);
    p_drv->write_reg(p_dev, 0xD504, 0x00);
    p_drv->write_reg(p_dev, 0xD505, 0x3A);
    p_drv->write_reg(p_dev, 0xD506, 0x00);
    p_drv->write_reg(p_dev, 0xD507, 0x4A);
    p_drv->write_reg(p_dev, 0xD508, 0x00);
    p_drv->write_reg(p_dev, 0xD509, 0x5C);
    p_drv->write_reg(p_dev, 0xD50A, 0x00);
    p_drv->write_reg(p_dev, 0xD50B, 0x81);

    p_drv->write_reg(p_dev, 0xD50C, 0x00);
    p_drv->write_reg(p_dev, 0xD50D, 0xA6);
    p_drv->write_reg(p_dev, 0xD50E, 0x00);
    p_drv->write_reg(p_dev, 0xD50F, 0xE5);
    p_drv->write_reg(p_dev, 0xD510, 0x01);
    p_drv->write_reg(p_dev, 0xD511, 0x13);
    p_drv->write_reg(p_dev, 0xD512, 0x01);
    p_drv->write_reg(p_dev, 0xD513, 0x54);
    p_drv->write_reg(p_dev, 0xD514, 0x01);
    p_drv->write_reg(p_dev, 0xD515, 0x82);
    p_drv->write_reg(p_dev, 0xD516, 0x01);
    p_drv->write_reg(p_dev, 0xD517, 0xCA);
    p_drv->write_reg(p_dev, 0xD518, 0x02);
    p_drv->write_reg(p_dev, 0xD519, 0x00);
    p_drv->write_reg(p_dev, 0xD51A, 0x02);
    p_drv->write_reg(p_dev, 0xD51B, 0x01);
    p_drv->write_reg(p_dev, 0xD51C, 0x02);
    p_drv->write_reg(p_dev, 0xD51D, 0x34);
    p_drv->write_reg(p_dev, 0xD51E, 0x02);
    p_drv->write_reg(p_dev, 0xD51F, 0x67);
    p_drv->write_reg(p_dev, 0xD520, 0x02);
    p_drv->write_reg(p_dev, 0xD521, 0x84);
    p_drv->write_reg(p_dev, 0xD522, 0x02);
    p_drv->write_reg(p_dev, 0xD523, 0xA4);
    p_drv->write_reg(p_dev, 0xD524, 0x02);
    p_drv->write_reg(p_dev, 0xD525, 0xB7);
    p_drv->write_reg(p_dev, 0xD526, 0x02);
    p_drv->write_reg(p_dev, 0xD527, 0xCF);
    p_drv->write_reg(p_dev, 0xD528, 0x02);
    p_drv->write_reg(p_dev, 0xD529, 0xDE);
    p_drv->write_reg(p_dev, 0xD52A, 0x02);
    p_drv->write_reg(p_dev, 0xD52B, 0xF2);
    p_drv->write_reg(p_dev, 0xD52C, 0x02);
    p_drv->write_reg(p_dev, 0xD52D, 0xFE);
    p_drv->write_reg(p_dev, 0xD52E, 0x03);
    p_drv->write_reg(p_dev, 0xD52F, 0x10);
    p_drv->write_reg(p_dev, 0xD530, 0x03);
    p_drv->write_reg(p_dev, 0xD531, 0x33);
    p_drv->write_reg(p_dev, 0xD532, 0x03);
    p_drv->write_reg(p_dev, 0xD533, 0x6D);
    p_drv->write_reg(p_dev, 0xD600, 0x00);
    p_drv->write_reg(p_dev, 0xD601, 0x33);
    p_drv->write_reg(p_dev, 0xD602, 0x00);
    p_drv->write_reg(p_dev, 0xD603, 0x34);
    p_drv->write_reg(p_dev, 0xD604, 0x00);
    p_drv->write_reg(p_dev, 0xD605, 0x3A);
    p_drv->write_reg(p_dev, 0xD606, 0x00);
    p_drv->write_reg(p_dev, 0xD607, 0x4A);
    p_drv->write_reg(p_dev, 0xD608, 0x00);
    p_drv->write_reg(p_dev, 0xD609, 0x5C);
    p_drv->write_reg(p_dev, 0xD60A, 0x00);
    p_drv->write_reg(p_dev, 0xD60B, 0x81);

    p_drv->write_reg(p_dev, 0xD60C, 0x00);
    p_drv->write_reg(p_dev, 0xD60D, 0xA6);
    p_drv->write_reg(p_dev, 0xD60E, 0x00);
    p_drv->write_reg(p_dev, 0xD60F, 0xE5);
    p_drv->write_reg(p_dev, 0xD610, 0x01);
    p_drv->write_reg(p_dev, 0xD611, 0x13);
    p_drv->write_reg(p_dev, 0xD612, 0x01);
    p_drv->write_reg(p_dev, 0xD613, 0x54);
    p_drv->write_reg(p_dev, 0xD614, 0x01);
    p_drv->write_reg(p_dev, 0xD615, 0x82);
    p_drv->write_reg(p_dev, 0xD616, 0x01);
    p_drv->write_reg(p_dev, 0xD617, 0xCA);
    p_drv->write_reg(p_dev, 0xD618, 0x02);
    p_drv->write_reg(p_dev, 0xD619, 0x00);
    p_drv->write_reg(p_dev, 0xD61A, 0x02);
    p_drv->write_reg(p_dev, 0xD61B, 0x01);
    p_drv->write_reg(p_dev, 0xD61C, 0x02);
    p_drv->write_reg(p_dev, 0xD61D, 0x34);
    p_drv->write_reg(p_dev, 0xD61E, 0x02);
    p_drv->write_reg(p_dev, 0xD61F, 0x67);
    p_drv->write_reg(p_dev, 0xD620, 0x02);
    p_drv->write_reg(p_dev, 0xD621, 0x84);
    p_drv->write_reg(p_dev, 0xD622, 0x02);
    p_drv->write_reg(p_dev, 0xD623, 0xA4);
    p_drv->write_reg(p_dev, 0xD624, 0x02);
    p_drv->write_reg(p_dev, 0xD625, 0xB7);
    p_drv->write_reg(p_dev, 0xD626, 0x02);
    p_drv->write_reg(p_dev, 0xD627, 0xCF);
    p_drv->write_reg(p_dev, 0xD628, 0x02);
    p_drv->write_reg(p_dev, 0xD629, 0xDE);
    p_drv->write_reg(p_dev, 0xD62A, 0x02);
    p_drv->write_reg(p_dev, 0xD62B, 0xF2);
    p_drv->write_reg(p_dev, 0xD62C, 0x02);
    p_drv->write_reg(p_dev, 0xD62D, 0xFE);
    p_drv->write_reg(p_dev, 0xD62E, 0x03);
    p_drv->write_reg(p_dev, 0xD62F, 0x10);
    p_drv->write_reg(p_dev, 0xD630, 0x03);
    p_drv->write_reg(p_dev, 0xD631, 0x33);
    p_drv->write_reg(p_dev, 0xD632, 0x03);
    p_drv->write_reg(p_dev, 0xD633, 0x6D);
    //LV2 Page 0 enable
    p_drv->write_reg(p_dev, 0xF000, 0x55);
    p_drv->write_reg(p_dev, 0xF001, 0xAA);
    p_drv->write_reg(p_dev, 0xF002, 0x52);
    p_drv->write_reg(p_dev, 0xF003, 0x08);
    p_drv->write_reg(p_dev, 0xF004, 0x00);
    //Display control
    p_drv->write_reg(p_dev, 0xB100, 0xCC);
    p_drv->write_reg(p_dev, 0xB101, 0x00);
    //Source hold time
    p_drv->write_reg(p_dev, 0xB600, 0x05);
    //Gate EQ control
    p_drv->write_reg(p_dev, 0xB700, 0x70);
    p_drv->write_reg(p_dev, 0xB701, 0x70);
    //Source EQ control (Mode 2)
    p_drv->write_reg(p_dev, 0xB800, 0x01);
    p_drv->write_reg(p_dev, 0xB801, 0x03);
    p_drv->write_reg(p_dev, 0xB802, 0x03);
    p_drv->write_reg(p_dev, 0xB803, 0x03);
    //Inversion mode (2-dot)
    p_drv->write_reg(p_dev, 0xBC00, 0x02);
    p_drv->write_reg(p_dev, 0xBC01, 0x00);
    p_drv->write_reg(p_dev, 0xBC02, 0x00);
    //Timing control 4H w/ 4-delay
    p_drv->write_reg(p_dev, 0xC900, 0xD0);
    p_drv->write_reg(p_dev, 0xC901, 0x02);
    p_drv->write_reg(p_dev, 0xC902, 0x50);
    p_drv->write_reg(p_dev, 0xC903, 0x50);
    p_drv->write_reg(p_dev, 0xC904, 0x50);
    p_drv->write_reg(p_dev, 0x3500, 0x00);
    p_drv->write_reg(p_dev, 0x3A00, 0x55);  //16-bit/pixel
    p_drv->write_cmd(p_dev, 0x1100);
    delay_us(120);
    p_drv->write_cmd(p_dev, 0x2900);

    // 5.3 亮背光
    gpio_output_set(&p_dev->ctrl_fsmc.back_light, 1);

    // 5.4 设置LCD扫描方向，清屏
    LCD_Scan_Dir(p_drv);
    LCD_Clear(p_drv);

    return 0;
}


