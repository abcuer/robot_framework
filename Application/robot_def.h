#ifndef ROBOT_DEF_H
#define ROBOT_DEF_H

// 底盘参数(单位：mm)
#define R_WHEEL 50.0f
#define REDUCTION_RATIO_WHEEL 19.0f
#define YAW_ALIGN_ANGLE 1000

// 总模式
typedef enum
{
    ROBOT_STOP = 0,
    ROBOT_READY,
}robot_mode_e;

typedef enum 
{
    CHASSIS_NONE_FORCE = 0, // 无力模式：遥控器怎么运动都是让机器人停下来
    CHASSIS_ROTATE,         // 旋转模式
    CHASSIS_NORMAL,         // 正常模式
}chassis_mode_e; // 底盘模式

// 底盘控制信息
typedef struct 
{
    float vx;   // 前后速度
    float vy;   // 左右速度
    float wz;   // 旋转速度
    chassis_mode_e chassis_mode;
    float offset_angle;
}chassis_ctrl_t; // 底盘控制

/* 底盘信息反馈给cmd控制中心，cmd在给mini PC发送数据 */
typedef struct 
{
    float real_vx; // 脉轮解算出真实速度后交给巡航
    float real_vy;
    float real_wz;
}chassis_feedback_t;

typedef enum 
{
    GIMBAL_NONE_FORCE = 0, // 无力模式：遥控器怎么运动都是让机器人停下来
    GIMBAL_NORMAL,         // 正常模式
}gimbal_mode_e; // 云台模式
typedef struct
{
    gimbal_mode_e gimbal_mode;
    float yaw;
    float pitch;
}gimbal_ctrl_t; 

typedef struct
{
    uint16_t yaw_motor_single_round_angle;
}gimbal_feedback_t;

typedef enum
{
    SHOOT_OFF = 0,
    SHOOT_ON,
}shoot_mode_e;

typedef enum  // 摩擦轮
{
    FRI_OFF = 0,
    FRI_ON,
}fri_mode_e;

typedef enum
{
    LOADER_OFF = 0,
    LOADER_NORMAL,
    LOADER_REVERSE,
}loader_mode_e;

typedef enum
{
    BULLET_15 = 15,
    BULLET_18 = 18,
    BULLET_30 = 30,
}bullet_speed_e;

typedef struct 
{
    shoot_mode_e shoot_mode;
    fri_mode_e fri_mode;
    loader_mode_e loader_mode;
    bullet_speed_e bullet_speed;
}shoot_ctrl_t;

typedef struct 
{
    float loader_speed;
}shoot_feedback_t;

#endif // !ROBOT_DEF_H