#ifndef BOARD_COMM_H
#define BOARD_COMM_H
#include "bsp_can.h"

#define CAN_COMM_BUFF_SIZE 60
#define CAN_COMM_HEADER 's'
#define CAN_COMM_TAIL 'e'
#define CAN_COMM_OFFSET_BYTES 4

// 帧头+数据帧+帧尾+校验
typedef struct
{
    CAN_INSTANCE_t *board_can_comm;
    /* 发送部分 */
    uint8_t send_data_len;  // 发送数据帧长度
    uint8_t send_buff_len;
    uint8_t raw_send_buff[CAN_COMM_BUFF_SIZE + CAN_COMM_OFFSET_BYTES];
    /* 接收部分 */
    uint8_t recv_data_len;  // 接收数据帧长度
    uint8_t recv_buff_len;
    uint8_t raw_recv_buff[CAN_COMM_BUFF_SIZE + CAN_COMM_OFFSET_BYTES];
    uint8_t unpacked_recv_buff[CAN_COMM_BUFF_SIZE];

    uint8_t update_flag;
}BOARD_CANcomm_INSTANCE_t;

typedef struct
{
    CAN_INIT_INSTANCE_t can_config;
    /* 发送部分 */
    uint8_t send_data_len;  // 发送数据帧长度
    uint8_t send_buff_len;
    /* 接收部分 */
    uint8_t recv_data_len;  // 接收数据帧长度
    uint8_t recv_buff_len;
}BOARD_INIT_CANcomm_INSTANCE_t;

BOARD_CANcomm_INSTANCE_t *board_comm_init(BOARD_INIT_CANcomm_INSTANCE_t *config);
void BOARD_can_send(BOARD_CANcomm_INSTANCE_t *instance, uint8_t *data);
void *Board_CAN_Get(BOARD_CANcomm_INSTANCE_t *instance);

#endif // !BOARD_COMM_H