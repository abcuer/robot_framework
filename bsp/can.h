#ifndef CAN_H
#define CAN_H

#include "stdint.h"

typedef struct 
{
    uint32_t id;
    uint32_t tx_id;
    uint32_t rx_id;
    uint8_t tx_buff[8];
    uint8_t rx_buff[8];
    uint32_t rx_len;
}CAN_INSTANCE_t;

typedef struct
{
    uint32_t id;
    uint32_t tx_id;
    uint32_t rx_id;
}CAN_INIT_INSTANCE_t;

#endif