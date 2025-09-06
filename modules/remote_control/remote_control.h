#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H 
#include "stdint.h"

#define RC_SW_UP    ((uint16_t)1)
#define RC_SW_MID   ((uint16_t)3)
#define RC_SW_DOWM  ((uint16_t)2)

#define switch_is_up(s)   (s == RC_SW_UP)
#define switch_is_mid(s)  (s == RC_SW_MID)
#define switch_is_down(s) (s == RC_SW_DOWM)

#define RC_CH_OFFSET 1024
typedef struct
{
    int16_t rocker_left_x;
    int16_t rocker_left_y;
    int16_t rocker_right_x;
    int16_t rocker_right_y;

    uint8_t switch_left;
    uint8_t switch_right;
}rc_ctrl_t;



#endif // !REMOTE_CONTROL_H
