#ifndef ROBOT_DEF_H
#define ROBOT_DEF_H

// 底盘参数(单位：mm)
#define R_WHEEL 50.0f
#define REDUCTION_RATIO_WHEEL 19.0f

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
}chassis_ctrl_t; // 底盘控制

/* 底盘信息反馈给cmd控制中心，cmd在给mini PC发送数据 */
typedef struct 
{
    float real_vx; // 脉轮解算出真实速度后交给巡航
    float real_vy;
    float real_wz;
}chassis_feedback_t;


#endif // !ROBOT_DEF_H