#include "shoot.h"
#include "DJMotor.h"
#include "robot_def.h"
#include "message_center.h"

static DJMotor_INSTANCE_t *fri_l_motor, *fri_r_motor, *loader;
static shoot_ctrl_t shoot_cmd_recv;
static shoot_feedback_t shoot_feedback_data;

static Publisher_t *shoot_pub;
static Subscriber_t *shoot_sub;

void shoot_init()
{
    motor_init_instance_t fri_l_config = {
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
        },
        .motor_type = M3508,
    };
    motor_init_instance_t fri_r_config = {
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
        },
        .motor_type = M3508,
    };

    motor_init_instance_t loader_config = {
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
        .motor_type = M2006,
    };
    fri_l_config.can_init_config.tx_id = 0x201;
    fri_l_config.motor_setting_config.motor_reverse_flag = MOTOR_NORMAL;
    fri_l_motor = DJMotorInit(&fri_l_config);

    fri_r_config.can_init_config.tx_id = 0x202;
    fri_r_config.motor_setting_config.motor_reverse_flag = MOTOR_REVERSE;
    fri_r_motor = DJMotorInit(&fri_r_config);

    loader_config.can_init_config.tx_id = 0x203;
    loader_config.motor_setting_config.motor_reverse_flag = MOTOR_REVERSE;
    loader = DJMotorInit(&loader_config);

    shoot_sub = SubRegister("shoot_cmd", sizeof(shoot_feedback_t));
    shoot_pub = PubRegister("shoot_feed", sizeof(shoot_ctrl_t));
}

static void shoot_mode_set()
{
    switch(shoot_cmd_recv.shoot_mode)
    {
        case SHOOT_OFF:
            DJMotor_Stop(&fri_l_motor);
            DJMotor_Stop(&fri_r_motor);
            DJMotor_Stop(&loader);
            break;
        case SHOOT_ON: 
            DJMotor_Enable(&fri_l_motor);
            DJMotor_Enable(&fri_r_motor);
            DJMotor_ENABLE(&loader);
            break;
        default:
            break;
    }
}

static void bullet_speed_set()
{
    switch(shoot_cmd_recv.loader_mode)
    {
        case BULLET_15:
            DJMotor_set(&fri_l_motor, 26000);
            DJMotor_set(&fri_r_motor, 26000);
            break;
        case BULLET_18:
            DJMotor_set(&fri_l_motor, 26000);
            DJMotor_set(&fri_r_motor, 26000);
            break;
        case BULLET_30:
            DJMotor_set(&fri_l_motor, 26000);
            DJMotor_set(&fri_r_motor, 26000);
            break;
        default:
    }
}

static void bullet_rate_set()
{
    switch(shoot_cmd_recv.bullet_speed)
    {
        case LOADER_OFF:
            DJMotor_set(&loader, 0);
            break;
        case LOADER_NORMAL:
            DJMotor_set(&loader, 1000);
            break;
        case LOADER_REVERSE:
            DJMotor_set(&loader, -500);
            break;
        default:
            DJMotor_set(&loader, 0);
            break;
    }
}

void shoot_task()
{
    SubGetMessage(shoot_sub, &shoot_cmd_recv);
    shoot_mode_set();  
    bullet_speed_set();
    bullet_rate_set();
    PubPushMessage(shoot_pub, (void *)&shoot_feedback_data);
}