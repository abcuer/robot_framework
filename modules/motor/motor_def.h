#ifndef MOTOR_DEF_H
#define MOTOR_DEF_H
#include "pid.h"
#include "can.h"

typedef enum    // 电机正反转
{
    MOTOR_NORMAL = 0,
    MOTOR_REVERSE,
}motor_reverse_flag_e;

typedef enum    // 电机使能
{
    MOTOR_STOP = 0,
    MOTOR_ENABLE,
}motor_enable_flag_e;

typedef enum    // 电机类型
{
    NONE = 0,
    GM6020,          // gimbal(云台)
    M3508,          // chassis(底盘)
    M2006,          // shooter(拨盘)
}motor_type_e;

typedef enum   // 闭环类型
{
    OPEN_LOOP = 0b0000,
    CURRENT_LOOP = 0b0001,      // 电流环
    ANGLE_LOOP = 0b0010,     // 位置环
    SPEED_LOOP = 0b0100,        // 速度环
    
    ANGLE_LOOP_AND_CURRENT_LOOP = ANGLE_LOOP | CURRENT_LOOP, // 0b0011
    CURRENT_LOOP_AND_SPEED_LOOP = CURRENT_LOOP | SPEED_LOOP, // 0b0101
    ANGLE_LOOP_AND_SPEED_LOOP = ANGLE_LOOP | SPEED_LOOP, // 0b0110
    ALL = CURRENT_LOOP | ANGLE_LOOP | SPEED_LOOP, // 0b0111,
}close_loop_type_e;

typedef struct
{
    float *other_angle_feedback_ptr;
    float *other_speed_feedback_ptr;
    
    PID_init_config_t speed_pid;
    PID_init_config_t angle_pid;
    PID_init_config_t current_pid;
    float set;
}pid_controller_t;

typedef struct
{
    float *other_angle_feedback_ptr;
    float *other_speed_feedback_ptr;
    
    PID_init_config_t speed_pid;
    PID_init_config_t angle_pid;
    PID_init_config_t current_pid;
}pid_init_controller_t;

typedef enum    // 反馈类型
{
    CURRENT_FEEDFOREWARD = 0b00,
    SPEED_FEEDFOREWARD = 0b01,
    LOCATION_FEEDFOREWARD = 0b10,
    LOCATION_SPEED_FEEDFOREWARD = 0b11,
}feedward_type_e;

typedef enum    // 电机反馈来源
{
    MOTOR_FEED = 0,
    OTHER_FEED,
}feedback_source_e;

typedef struct
{
    close_loop_type_e outer_loop_type;
    close_loop_type_e close_loop_type;
    feedback_source_e angle_feedback_source;
    feedback_source_e speed_feedback_source;
    motor_reverse_flag_e motor_reverse_flag;
    motor_enable_flag_e motor_enable_flag;
}motor_setting_t;

typedef struct 
{
    pid_init_controller_t pid_init_config;
    motor_setting_t motor_setting_config;
    motor_type_e motor_type;
    CAN_INIT_INSTANCE_t can_init_config;
}motor_init_instance_t;





#endif // !MOTOR_DEF_H