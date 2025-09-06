#include "robot_def.h"
#include "cmd.h"
#include "remote_control.h"

static chassis_ctrl_t chassis_cmd_send;
static chassis_feedback_t chassis_fetch_data;
static rc_ctrl_t rc_data;

static remote_control_set()
{
    if(switch_is_down(rc_data.weitch_right))
    chassis_cmd_send.chassis_mode = CHASSIS_NONE_FORCE;
    else if(switch_is_mid(rc_data.weitch_right))
    chassis_cmd_send.chassis_mode = CHASSIS_NORMAL;
    else if(switch_is_up(rc_data.weitch_right))
    chassis_cmd_send.chassis_mode = CHASSIS_ROTATE;

    chassis_cmd_send.vx = rc_data.rocker_left_y;
    chassis_cmd_send.vy = -rc_data.rocker_left_x;
    
}

static void chassis_motor_state_set()
{/*
    if(chassis_cmd_recv.chassis_mode = CHASSIS_NONE_FORCE)

    else
*/
}

static void rotate_speed_set()
{
    switch(chassis_cmd_recv.chassis_mode)
    {
        case CHASSIS_ROTATE:
            chassis_cmd_recv.wz = 1200;
            break;
        case CHASSIS_NORMAL:
            chassis_cmd_recv.wz = 0;
            break;
        default:
            chassis_cmd_recv.wz = 0;
            break;
    }
}

static void malun_cal()
{
    v_lf = chassis_cmd_recv.vx - chassis_cmd_recv.vy + chassis_cmd_recv.wz;
    v_lb = chassis_cmd_recv.vx + chassis_cmd_recv.vy + chassis_cmd_recv.wz;
    v_rf = chassis_cmd_recv.vx + chassis_cmd_recv.vy - chassis_cmd_recv.wz;
    v_rb = chassis_cmd_recv.vx - chassis_cmd_recv.vy + chassis_cmd_recv.wz;
/*
    chassis_fetch_data.real_vx = 
    chassis_fetch_data.real_vy =
    chassis_fetch_data.real_wz =
*/
}