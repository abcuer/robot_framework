#include "robot_def.h"
#include "cmd.h"
#include "remote_control.h"
#include "board_comm.h"
#include "message_center.h"

static chassis_ctrl_t chassis_cmd_send;
static chassis_feedback_t chassis_fetch_data;
static rc_ctrl_t rc_data;
static BOARD_CANcomm_INSTANCE_t *cmd_can_comm;

void ROBOT_CMD_Init()
{
    BOARD_INIT_CANcomm_INSTANCE_t comm_conf=
    {
        .can_config = {
            .tx_id = 0x200, 
            .rx_id = 0x209,
        },
        .send_data_len = sizeof(chassis_ctrl_t),
        .recv_data_len = sizeof(chassis_feedback_t),
    };
    cmd_can_comm = board_comm_init(&comm_conf);
}
static void remote_control_set()
{
    if(switch_is_down(rc_data.switch_right))
    chassis_cmd_send.chassis_mode = CHASSIS_NONE_FORCE;
    else if(switch_is_mid(rc_data.switch_right))
    chassis_cmd_send.chassis_mode = CHASSIS_NORMAL;
    else if(switch_is_up(rc_data.switch_right))
    chassis_cmd_send.chassis_mode = CHASSIS_ROTATE;

    chassis_cmd_send.vx = rc_data.rocker_left_y;
    chassis_cmd_send.vy = -rc_data.rocker_left_x;
}

void cmd_task()
{
    chassis_fetch_data = *(chassis_feedback_t *)Board_CAN_Get(cmd_can_comm);
    remote_control_set();  
    BOARD_can_send(cmd_can_comm, (void *)&chassis_cmd_send);

}