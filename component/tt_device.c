#include "tt_device.h"
#include <string.h>

static tt_list_t *tt_device_list = NULL;
static tt_list_t *head = NULL;

static int tt_device_find(tt_device_t *device)
{
    tt_list_t *node = head;
    while (node != NULL) {
        tt_device_t *current_device = GET_DEVICE_FROM_NODE(node, node);
        if (current_device == device) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void tt_device_add(tt_device_t *device)
{
    if (device == NULL) {
        return;
    }
    if (tt_device_list == NULL) {   
        tt_device_list = &device->node;
        head = &device->node;
    } else {
        if(tt_device_find(device)) return;
        tt_device_list->next = &device->node;
        tt_device_list = &device->node;
    }
}

void tt_device_remove(tt_device_t *device)
{
    if (device == NULL) {
        return;
    }
    if (tt_device_list == NULL) {
        return;
    }
    if (tt_device_list == &device->node) {
        tt_device_list = tt_device_list->next;
        head = tt_device_list;
    } else {
        tt_list_t *node = head;
        while (node->next!= NULL) {
            if (node->next == &device->node) {
                node->next = node->next->next;
                break;
            }
            node = node->next;
        }
    }
}

tt_device_t *tt_device_get_by_name(const char *name)
{
    tt_list_t *node = head;
    while (node!= NULL) {
        tt_device_t *device = GET_DEVICE_FROM_NODE(node,node);
        if (device->name!= NULL && strcmp(device->name, name) == 0) {
            return device;
        }
        node = node->next;
    }
    return NULL;
}

