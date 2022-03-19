#ifndef __F0_FLASH_H
#define __F0_FLASH_H

#ifdef FT32
#include "ft32f0xx.h"
#endif 

#ifdef HK32F0
#include "hk32f0xx.h"
#endif

int f0_flash_write_word(uint32_t address, uint16_t len, uint32_t *p_data);
int f0_flash_read_word(uint32_t address, uint16_t len, uint32_t *p_data);

int f0_flash_read(uint32_t address, uint16_t len, uint8_t *p_data);
#endif

