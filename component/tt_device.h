#ifndef TT_DEVICE_H
#define TT_DEVICE_H
#include <stdint.h>
#include "tt_list.h"
#include <stddef.h>

typedef struct tt_device{
    char *name;
    int (*init)(struct tt_device *dev);
    int (*deinit)(struct tt_device *dev);
    int (*suspend)(struct tt_device *dev);
    int (*resume)(struct tt_device *dev);
    tt_list_node_t node;
    void *priv;
} tt_device_t;

#define GET_DEVICE_FROM_NODE(ptr,member) \
  ((tt_device_t *)((char *)(ptr) - (uintptr_t)(&((tt_device_t *)0)->member)))

void tt_device_add(tt_device_t *dev);
void tt_device_remove(tt_device_t *dev);
tt_device_t *tt_device_get_by_name(const char *name);



#endif
