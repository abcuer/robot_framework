#include "DJMotor.h"
#include <stddef.h>

int idx = 0;
static DJMotor_INSTANCE_t *djmotor_instance[DJMotor_num] = {0};
static CAN_INSTANCE_t send_to_can[6] =
{
    [0] = {.tx_buff = {0}}
};

void DJMotor_Stop(DJMotor_INSTANCE_t *motor)
{
    motor->motor_setting.motor_enable_flag = MOTOR_STOP;
}

void DJMotor_Enable(DJMotor_INSTANCE_t *motor)
{
    motor->motor_setting.motor_enable_flag = MOTOR_ENABLE;
}

void DJMotor_outerloop(DJMotor_INSTANCE_t *motor, close_loop_type_e outer_loop)
{
    motor->motor_setting.outer_loop_type = outer_loop;
}

void DJMotor_Changefeed(DJMotor_INSTANCE_t *motor, feedback_source_e source, close_loop_type_e loop)
{
    if(loop == ANGLE_LOOP)
        motor->motor_setting.angle_feedback_source = source;
    else if(loop == SPEED_LOOP)
        motor->motor_setting.speed_feedback_source = source;
}

void DJMotor_set(DJMotor_INSTANCE_t *motor, float set)
{
    motor->motor_controller.set = set;
}

void Decode_DJMotor(CAN_INSTANCE_t *instance) // 解码电机数据
{
    uint8_t *rxbuff = instance->rx_buff;
    DJMotor_INSTANCE_t *motor = (DJMotor_INSTANCE_t *)instance->id;

    motor->measure.ecd = (uint16_t)(rxbuff[0]<<8) | rxbuff[1];
    motor->measure.last_ecd = motor->measure.ecd;
    motor->measure.angle_round = ECD_TO_ANGLE * motor->measure.ecd;
    motor->measure.speed_aps = + (1 - SPEED_SMOOTH_COEF)*(float)(uint16_t)(rxbuff[2]<<8|rxbuff[3]) + SPEED_SMOOTH_COEF * motor->measure.speed_aps;
    motor->measure.real_current = + (1 - CURRENT_SMOOTH_COEF)*(float)(uint16_t)(rxbuff[4]<<8|rxbuff[5]) + SPEED_SMOOTH_COEF * motor->measure.real_current;
    motor->measure.temperature = rxbuff[6];

    if(motor->measure.ecd - motor->measure.last_ecd > 4096)
        motor->measure.total_round --;
    else if(motor->measure.ecd - motor->measure.last_ecd < -4096)
        motor->measure.total_round ++;
    motor->measure.total_angle = motor->measure.total_round * 360.0f + motor->measure.angle_round;
}

DJMotor_INSTANCE_t *DJMotorInit(motor_init_config *config)
{
    DJMotor_INSTANCE_t *instance;
    instance->motor_type = config->motor_type;
    instance->motor_setting.motor_reverse_flag = config->motor_setting_config.motor_reverse_flag;

    PID_Init(&instance->motor_controller.speed_pid, &config->pid_init_config.speed_pid);
    PID_Init(&instance->motor_controller.angle_pid, &config->pid_init_config.angle_pid);
    PID_Init(&instance->motor_controller.current_pid, &config->pid_init_config.current_pid);

    instance->motor_controller.other_angle_feedback_ptr = config->pid_init_config.other_angle_feedback_ptr;
    instance->motor_controller.other_speed_feedback_ptr = config->pid_init_config.other_speed_feedback_ptr;
    config->can_init_config.id = instance;

    DLMotor_Enable(instance);
    djmotor_instance[idx++] = instance;
    return instance;
}

void DJMotor_control()
{
    DJMotor_INSTANCE_t *motor;
    motor_setting_t *motor_setting;
    DJMOTOR_Measure_t *measure;
    pid_init_controller_t *motor_pid;
    float pid_set, pid_measure;
    int16_t message_num = motor->motor_can_instance->tx_id - 0x201;
    int16_t set;

    for(size_t i=0; i<idx; ++i)
    {
        motor = djmotor_instance[i];
        motor_setting = &motor->motor_setting;
        measure = &motor->measure;
        motor_pid = &motor->motor_controller;
        pid_set = motor_pid->set;
        
        if(motor->motor_setting.motor_reverse_flag == MOTOR_REVERSE)
            pid_set *= -1;
        if((motor_setting->close_loop_type&ANGLE_LOOP)&&(motor_setting->outer_loop_type == ANGLE_LOOP))
        {
            if(motor_setting->angle_feedback_source == OTHER_FEED)
                pid_measure = *motor_pid->other_angle_feedback_ptr;
            else
                pid_measure = measure->angle_round;
            pid_set = PID_Calculate(&motor_pid->angle_pid, pid_set, pid_measure);
        }  
        if((motor_setting->close_loop_type&SPEED_LOOP)&&(motor_setting->outer_loop_type == ANGLE_LOOP | SPEED_LOOP))
        {
            if(motor_setting->angle_feedback_source == OTHER_FEED)
                pid_measure = *motor_pid->other_speed_feedback_ptr;
            else
                pid_measure = measure->speed_aps;
            pid_set = PID_Calculate(&motor_pid->speed_pid, pid_set, pid_measure);
        }    
        if(motor_setting->close_loop_type&CURRENT_LOOP)
        {
            pid_set = PID_Calculate(&motor_pid->current_pid, pid_set, pid_measure);
        }  
        set = (int16_t)pid_set;
    }  
    send_to_can[0].tx_buff[2*message_num] = (uint8_t)(set>>8);
    send_to_can[0].tx_buff[2*message_num + 1] = (uint8_t)(set&0x00ff);
    if(motor->motor_setting.motor_enable_flag == MOTOR_STOP)
    {
        memset(send_to_can[0].tx_buff+2*message_num, 0, 16u);
    }
    
}
