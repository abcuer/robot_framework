#include "gimbal.h"
#include "DJMotor.h"
#include "robot_def.h"
#include "message_center.h"

static DJMotor_INSTANCE_t *yaw_motor, *pitch_motor;
static gimbal_ctrl_t gimbal_cmd_recv;
static gimbal_feedback_t gimbal_feedback_data;

static Publisher_t *gimbal_pub;
static Subscriber_t *gimbal_sub;

void gimbal_init()
{
    motor_init_instance_t yaw_config = {
        .can_init_config = {
            .tx_id = 2,
        },
        .pid_init_config = {
            .angle_pid = {
                .kp = 3,
                .ki = 3, 
                .kd = 0.1,
                .maxout = 500,
            },
            .speed_pid = {
                .kp = 260,
                .ki = 0,
                .kd = 0,
                .maxout = 20000,
            },
            // .other_speed_feedback_ptr = &gimbal_imu->gyro[2],
        },
        .motor_setting_config = {
            .angle_feedback_source = OTHER_FEED,
            .speed_feedback_source = OTHER_FEED,
            .close_loop_type = ANGLE_LOOP | SPEED_LOOP,
            .outer_loop_type = ANGLE_LOOP,
            .motor_reverse_flag = MOTOR_REVERSE,
        },
        .motor_type = GM6020,
    };
    motor_init_instance_t pitch_config = {
        .can_init_config = {
            .tx_id = 1,
        },
        .pid_init_config = {
            .angle_pid = {
                .kp = 3,
                .ki = 3, 
                .kd = 0.1,
                .maxout = 500,
            },
            .speed_pid = {
                .kp = 260,
                .ki = 0,
                .kd = 0,
                .maxout = 20000,
            },
        },
        .motor_setting_config = {
            .angle_feedback_source = OTHER_FEED,
            .speed_feedback_source = OTHER_FEED,
            .close_loop_type = ANGLE_LOOP | SPEED_LOOP,
            .outer_loop_type = ANGLE_LOOP,
            .motor_reverse_flag = MOTOR_REVERSE,
        },
        .motor_type = GM6020,
    };
    gimbal_sub = SubRegister("gimbal_cmd", sizeof(gimbal_feedback_t));
    gimbal_pub = PubRegister("gimbal_feed", sizeof(gimbal_ctrl_t));
}

static void gimbal_mode_set()
{
    switch(gimbal_cmd_recv.gimbal_mode)
    {
        case GIMBAL_NONE_FORCE:
            DJMotor_Stop(&yaw_motor);
            DJMotor_Stop(&pitch_motor);
            break;
        case GIMBAL_NORMAL: 
            DJMotor_Enable(&yaw_motor);
            DJMotor_Enable(&pitch_motor);
            DJMotor_set(&yaw_motor, gimbal_cmd_recv.yaw);
            DJMotor_set(&pitch_motor, gimbal_cmd_recv.pitch);
            break;
        default:
            break;
    }
}

static void gimbal_mode_set()
{

}

void gimbal_task()
{
    SubGetMessage(gimbal_sub, &gimbal_cmd_recv);
    gimbal_mode_set();  
    PubPushMessage(gimbal_pub, (void *)&gimbal_feedback_data);
}