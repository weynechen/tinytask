#ifndef _QUICKLZ_DEC_
#define _QUICKLZ_DEC_
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef int32_t (*fdecompress_cb)(uint8_t *decompress_data, uint32_t len, uint16_t packCnt);
typedef int32_t (*read_flash_cb)(uint8_t *addr, uint8_t *buf, uint32_t len);

uint32_t quicklz_decompress(uint8_t *pack_addr, uint32_t pack_size, fdecompress_cb write_cb, read_flash_cb read_cb);

#endif
