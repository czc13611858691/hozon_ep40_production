#ifndef __touch_press_motor_H_
#define __touch_press_motor_H_

#pragma once

#include <stdint.h>

typedef void (*motor_shake_cb_t)(void);

typedef struct
{
    uint8_t state_handle;
    uint8_t slide_trig_flg;//主驾特色，震动对应4个，信号对应8个

    uint8_t adc_res_index;
    uint32_t adc_res;
    uint32_t adc_min;
    uint32_t adc_max;
    uint16_t delta_D1;
    uint16_t delta_D2;
    uint16_t delta_D3;
    uint8_t capsense_status;
    motor_shake_cb_t motor_shake;
    uint16_t motor_shake_jitter_period;
} state_obj_t;

/* 配置按键 */
typedef enum
{
    BTN_SELF_DEF,
    BTN_B,
    BTN_A,
    SLIDE_1_DIR_TRIG,
    // SLIDE_2_DIR_TRIG,
    BTN_NUM,
} btn_e;

extern state_obj_t g_btn_state[BTN_NUM];

void state_mahine_run(state_obj_t *state_obj_ptr);

#endif