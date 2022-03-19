#ifndef __BAND_FREQ_H
#define __BAND_FREQ_H

#define BK9532_FREQ_632_MHZ    0x4D2B1EB8
#define BK9532_FREQ_660_MHZ    0x50961EB8
#define BK9532_FREQ_0_3_MHZ    0x96000//0xE7EB8

#define BK9531_FREQ_632_MHZ    0x4D260000
#define BK9531_FREQ_660_MHZ    0x50910000
#define BK9531_FREQ_0_3_MHZ    0x96000

typedef enum
{
    BAND_TYPE_V,
    BAND_TYPE_U,
} band_type_e;

typedef enum
{
    REGION_BAND_DEFAULT,
    REGION_BAND_MAX,
} region_band_e;


#endif
