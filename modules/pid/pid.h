#ifndef PID_H
#define PID_H

typedef struct
{
    float kp;
    float ki;
    float kd;
    float maxout;
}PID_init_config_t;

typedef struct 
{
    float kp;
    float ki;
    float kd;
    float maxout;

    float set;
    float measure;
    float last_measure;
    float err;
    float last_err;

    float output;
    float last_output;

    float Pout;
    float Iout;
    float Dout;

    float dt;
}PID_instance_t;

void PID_Init(PID_instance_t *pid, PID_init_config_t *pid_init);
float PID_Calculate(PID_instance_t *pid, float set, float measure);

#endif // !PID_H