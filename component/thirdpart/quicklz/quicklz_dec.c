#include <stdio.h>
#include "quicklz_dec.h"
#include "FreeRTOS.h"
#include "app_log.h"
#include "quicklz.h"

#define DEC_BUF_SIZE (1024)
#define FLASH_PAGE_SIZE   (400)

#define DECOMPRESS_4K   (4*1024)

uint8_t tx_buffer[TX_AND_DECOMPRESS_BUFFER_SIZE];

uint32_t quicklz_decompress(uint8_t *pack_addr, uint32_t pack_size, fdecompress_cb cb, read_flash_cb read_cb)
{
    uint32_t counter4k = 0;
    uint32_t decompress4k = 0;
    int32_t result = 0;
    uint32_t compress_len = 0;
    uint32_t decompress_len = 0;
    uint32_t rd_pos = 0;
    uint32_t wr_pos = 0;
    uint32_t decompress_size = 0;
    uint32_t compress_size = 0;
    uint8_t *in_buffer = NULL;
    uint8_t *out_buffer = NULL;
    uint8_t *lv_share_buf = NULL;
    qlz_state_decompress *state_decompress = NULL;
    
    in_buffer = (uint8_t *)pvPortMalloc(DEC_BUF_SIZE + FLASH_PAGE_SIZE);
    if (in_buffer == NULL)
    {
        APP_LOG_ERROR("in_buffer malloc fail\n");
        goto exit;
    }
    
    result = 0;
    state_decompress = (qlz_state_decompress *)tx_buffer;
    lv_share_buf = tx_buffer + sizeof(qlz_state_decompress);
    out_buffer = lv_share_buf;

    memset(state_decompress, 0, sizeof(qlz_state_decompress));

    APP_LOG_ERROR("decompress pack_addr(%p) pack_size(%08x) buf_size: %d, start...\n", 
               pack_addr, pack_size, (sizeof(qlz_state_decompress) + DECOMPRESS_4K));
    
    while (compress_size < pack_size)
    {
        if ((compress_size + 9) > pack_size)
        {
            APP_LOG_ERROR("head last data %d %d.\r\n", decompress4k, pack_size - compress_size);
//          memcpy(out_buffer + decompress4k, (const void*)(pack_addr + rd_pos), pack_size - compress_size);
            read_cb((pack_addr + rd_pos), out_buffer + decompress4k, pack_size - compress_size);
            if(cb)
            {
                APP_LOG_ERROR("head wr 4k %d\n", decompress4k + pack_size - compress_size);
                cb(out_buffer + decompress4k, decompress4k + pack_size - compress_size, counter4k++);
            }

            compress_size += pack_size - compress_size;
            decompress_size += pack_size - compress_size;
        }
        else
        {
//            memcpy(in_buffer,(const void*)(pack_addr + rd_pos), 9);
            read_cb((pack_addr + rd_pos), in_buffer, 9);
            compress_len = *((uint32_t *) (in_buffer + 1));
            decompress_len = *((uint32_t *) (in_buffer + 5));
            if (decompress_len == 1024)
            {
//              memcpy(in_buffer,(const void*)(pack_addr + rd_pos), compress_len);

                read_cb((pack_addr + rd_pos), in_buffer, compress_len);
                qlz_decompress((char *) in_buffer, out_buffer + decompress4k, state_decompress);
                decompress4k += decompress_len;
                if(cb && decompress4k == DECOMPRESS_4K)
                {
                    APP_LOG_ERROR("wr 4k %d\n", decompress4k);
                    cb(out_buffer, decompress4k, counter4k++);
                    decompress4k = 0;
                }
                wr_pos += decompress_len;
                rd_pos += compress_len;
                compress_size += compress_len;
                decompress_size += decompress_len;
                APP_LOG_ERROR("d size is %d, c size is %d.\r\n", decompress_size, compress_size);
            }
            else
            {
                APP_LOG_ERROR("tail last data %d %d.\r\n", decompress4k, pack_size - compress_size);
             //   memcpy(out_buffer + decompress4k,(const void*)(pack_addr + rd_pos), pack_size - compress_size);
                read_cb((pack_addr + rd_pos), out_buffer + decompress4k, pack_size - compress_size);
                if(cb)
                {
                    APP_LOG_ERROR("tail wr 4k %d\n", decompress4k + pack_size - compress_size);
                    cb(out_buffer, decompress4k + pack_size - compress_size, counter4k++);
                }
                decompress_size += pack_size - compress_size;
                compress_size += pack_size - compress_size;
            }
        }
    }
    result = 1;

exit:

    if (in_buffer)
        vPortFree(in_buffer);

    if(result == 1)
        APP_LOG_ERROR("decompress pass c size is %d, d size is %d.\r\n", compress_size, decompress_size);
    else
        APP_LOG_ERROR("decompress fail.\r\n");
    return decompress_size;
}

