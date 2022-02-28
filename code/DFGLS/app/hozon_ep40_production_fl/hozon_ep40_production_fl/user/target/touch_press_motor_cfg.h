#ifndef __touch_press_motor_cfg_H_ 
#define __touch_press_motor_cfg_H_

#pragma once

#include <stdint.h>

/**
 * @brief 震动触发后会对adc采样产生一定的影响
 * TOUCH_PRESS_MOTOR_JITTER_PRESS_TICKS--按下按键后电机震动，屏蔽触摸震动的时间
 * TOUCH_PRESS_MOTOR_JITTER_RELEASE_TICKS--松开按键后电机震动，屏蔽触摸震动的时间
 * 
 */
#define TOUCH_PRESS_MOTOR_JITTER_PRESS_TICKS (2)
#define TOUCH_PRESS_MOTOR_JITTER_RELEASE_TICKS (5)

/**
 * @brief 配置按键的霍尔触发参数
 * 后缀D1    首次按下需要的压力变化
 * 后缀D2    松开按下需要的压力变化
 * 后缀D3    再次按下需要的压力变化
 * 
 */

#define BTN_A_DELTA_D1          400
#define BTN_A_DELTA_D2          380
#define BTN_A_DELTA_D3          380

#define BTN_B_DELTA_D1          400
#define BTN_B_DELTA_D2          380
#define BTN_B_DELTA_D3          380

#define BTN_SELF_DELTA_D1       400
#define BTN_SELF_DELTA_D2       380
#define BTN_SELF_DELTA_D3       380

#define SLIDE_DELTA_D1          400
#define SLIDE_DELTA_D2          380
#define SLIDE_DELTA_D3          380

extern uint8_t g_slide_shake_end_flg;

void soft_timer_touch_press_motor_task(void);

#endif