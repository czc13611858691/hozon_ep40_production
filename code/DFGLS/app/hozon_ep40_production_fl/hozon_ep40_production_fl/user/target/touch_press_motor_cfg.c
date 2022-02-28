/**
 * @file touch_press_motor_cfg.c
 * @author 曹志成 (czc13611858691@gmail.com)
 * @brief
 * 触摸震动的上层配置
 * @version 0.1
 * @date 2021-11-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "touch_press_motor_cfg.h"
#include "touch_press_motor.h"
#include "touch.h"
#include <stdio.h>
#include "led.h"
#include "driver_init.h"
#include "atmel_start_pins.h"
#include "adc.h"
#include "ep40_cfg.h"
#include "dac.h"

uint16_t g_motor_jitter_ticks = 0;

/* 触摸震动的回调函数 */
#define REGISTER_MOTOR(index)                                              \
    void motor_shake_cb##index(void)                                       \
    {                                                                      \
        static uint8_t shake_status = 0;                                   \
        shake_status = (shake_status + 1) % 2;                             \
        if (shake_status == 0)                                             \
        {                                                                  \
            dac_run_test();                                                \
            g_motor_jitter_ticks = TOUCH_PRESS_MOTOR_JITTER_RELEASE_TICKS; \
        }                                                                  \
        else                                                               \
        {                                                                  \
            dac_run();                                                     \
            g_motor_jitter_ticks = TOUCH_PRESS_MOTOR_JITTER_PRESS_TICKS;   \
            lin_signal_send_x_times(LI0_DFGLS_ButtonSoundRequest, 1, 1);   \
        }                                                                  \
    }

REGISTER_MOTOR(0);
REGISTER_MOTOR(1);
REGISTER_MOTOR(2);

#define BTN_A_ADC_INDEX (1)
#define BTN_B_ADC_INDEX (0)

uint8_t g_touch_press_motor_init_flg = 0;
uint8_t g_slide_shake_end_flg = 0;

void slide_shake_cb(void)
{
    static uint8_t shake_status = 0;
    shake_status = (shake_status + 1) % 2;

    if (shake_status == 0)
    {
        dac_run_test();
        g_motor_jitter_ticks = TOUCH_PRESS_MOTOR_JITTER_RELEASE_TICKS;
    }
    else
    {
        dac_run();
        g_slide_shake_end_flg = 1;
        // lin_signal_send_x_times(LI0_DFGLS_ButtonSoundRequest, 1, 1);
        g_motor_jitter_ticks = 2;
    }
}

void soft_timer_touch_press_motor_task(void)
{
    uint8_t btn_status[BTN_NUM] = { 0 };
    uint8_t adc_res_index = 0;

    if (g_touch_press_motor_init_flg == 0)
    {
        g_touch_press_motor_init_flg = 1;

        g_btn_state[BTN_SELF_DEF].motor_shake = motor_shake_cb0;
        adc_res_index = BTN_B_ADC_INDEX;
        g_btn_state[BTN_SELF_DEF].adc_res_index = adc_res_index;
        g_btn_state[BTN_SELF_DEF].adc_res = ADC_0_measurement[adc_res_index];
        g_btn_state[BTN_SELF_DEF].adc_min = 0xffffffff;
        g_btn_state[BTN_SELF_DEF].adc_max = 0;
        g_btn_state[BTN_SELF_DEF].delta_D1 = BTN_SELF_DELTA_D1;
        g_btn_state[BTN_SELF_DEF].delta_D2 = BTN_SELF_DELTA_D2;
        g_btn_state[BTN_SELF_DEF].delta_D3 = BTN_SELF_DELTA_D3;

        g_btn_state[BTN_B].motor_shake = motor_shake_cb1;
        adc_res_index = BTN_B_ADC_INDEX;
        g_btn_state[BTN_B].adc_res_index = adc_res_index;
        g_btn_state[BTN_B].adc_res = ADC_0_measurement[adc_res_index];
        g_btn_state[BTN_B].adc_min = 0xffffffff;
        g_btn_state[BTN_B].adc_max = 0;
        g_btn_state[BTN_B].delta_D1 = BTN_B_DELTA_D1;
        g_btn_state[BTN_B].delta_D2 = BTN_B_DELTA_D2;
        g_btn_state[BTN_B].delta_D3 = BTN_B_DELTA_D3;

        g_btn_state[BTN_A].motor_shake = motor_shake_cb2;
        adc_res_index = BTN_A_ADC_INDEX;
        g_btn_state[BTN_A].adc_res_index = adc_res_index;
        g_btn_state[BTN_A].adc_res = ADC_0_measurement[adc_res_index];
        g_btn_state[BTN_A].adc_min = 0xffffffff;
        g_btn_state[BTN_A].adc_max = 0;
        g_btn_state[BTN_A].delta_D1 = BTN_A_DELTA_D1;
        g_btn_state[BTN_A].delta_D2 = BTN_A_DELTA_D2;
        g_btn_state[BTN_A].delta_D3 = BTN_A_DELTA_D3;

        g_btn_state[SLIDE_1_DIR_TRIG].motor_shake = slide_shake_cb;
        adc_res_index = BTN_B_ADC_INDEX;
        g_btn_state[SLIDE_1_DIR_TRIG].adc_res_index = adc_res_index;
        g_btn_state[SLIDE_1_DIR_TRIG].adc_res = ADC_0_measurement[adc_res_index];
        g_btn_state[SLIDE_1_DIR_TRIG].adc_min = 0xffffffff;
        g_btn_state[SLIDE_1_DIR_TRIG].adc_max = 0;
        g_btn_state[SLIDE_1_DIR_TRIG].delta_D1 = SLIDE_DELTA_D1;
        g_btn_state[SLIDE_1_DIR_TRIG].delta_D2 = SLIDE_DELTA_D2;
        g_btn_state[SLIDE_1_DIR_TRIG].delta_D3 = SLIDE_DELTA_D3;

        // g_btn_state[SLIDE_2_DIR_TRIG].motor_shake = slide_shake_cb;
        // adc_res_index = BTN_A_ADC_INDEX;
        // g_btn_state[SLIDE_2_DIR_TRIG].adc_res_index = adc_res_index;
        // g_btn_state[SLIDE_2_DIR_TRIG].adc_res = g_adc_res[adc_res_index];
        // g_btn_state[SLIDE_2_DIR_TRIG].adc_min = 0xffffffff;
        // g_btn_state[SLIDE_2_DIR_TRIG].adc_max = 0;
        // g_btn_state[SLIDE_2_DIR_TRIG].delta_D1 = 250;
        // g_btn_state[SLIDE_2_DIR_TRIG].delta_D2 = 220;
        // g_btn_state[SLIDE_2_DIR_TRIG].delta_D3 = 220;
    }
    else
    {
        if (g_motor_jitter_ticks > 0)
        {
            g_motor_jitter_ticks--;
            return;
        }
        if (dac_ret_status() == 0)
        {
            for (uint8_t i = 0; i < BTN_NUM - 1; i++)
            {
                adc_res_index = g_btn_state[i].adc_res_index;

                g_btn_state[i].adc_res = ADC_0_measurement[adc_res_index];

                btn_status[i] = ((get_sensor_state(i) & 0x80) | (get_sensor_state(i + 3) & 0x80)) >> 7;
                if (((get_sensor_state(i) & 0x80) >> 7) != 0)
                {
                    g_btn_state[i].slide_trig_flg = 0;
                }
                else if (((get_sensor_state(i + 3) & 0x80) >> 7) != 0)
                {
                    g_btn_state[i].slide_trig_flg = 1;
                }

                g_btn_state[i].capsense_status = btn_status[i];
            }

            uint8_t slide_state = (get_scroller_state(0) | get_scroller_state(1));
            uint8_t slide_pos = 0;
            if (get_scroller_state(0) != 0)
            {
                slide_pos = get_scroller_position(0);
                g_btn_state[SLIDE_1_DIR_TRIG].slide_trig_flg = 0;
            }
            else if (get_scroller_state(1) != 0)
            {
                slide_pos = get_scroller_position(1);
                g_btn_state[SLIDE_1_DIR_TRIG].slide_trig_flg = 1;
            }

            // adc_res_index = g_btn_state[SLIDE_2_DIR_TRIG].adc_res_index;
            // g_btn_state[SLIDE_2_DIR_TRIG].adc_res = g_adc_res[adc_res_index];

            if (slide_state != 0)
            {
                if (slide_pos < 125)
                {
                    if (g_btn_state[SLIDE_1_DIR_TRIG].state_handle == 0)
                    {
                        // adc_res_index = BTN_B_ADC_INDEX;
                        g_btn_state[SLIDE_1_DIR_TRIG].adc_res_index = BTN_B_ADC_INDEX;
                        g_btn_state[SLIDE_1_DIR_TRIG].delta_D1 = BTN_B_DELTA_D1;
                        g_btn_state[SLIDE_1_DIR_TRIG].delta_D2 = BTN_B_DELTA_D2;
                        g_btn_state[SLIDE_1_DIR_TRIG].delta_D3 = BTN_B_DELTA_D3;
                        // if (g_btn_state[SLIDE_1_DIR_TRIG].state_handle == 0)
                        {
                            g_btn_state[SLIDE_1_DIR_TRIG].adc_res = g_btn_state[BTN_B].adc_res;
                            g_btn_state[SLIDE_1_DIR_TRIG].adc_min = g_btn_state[BTN_B].adc_min;
                            g_btn_state[SLIDE_1_DIR_TRIG].adc_max = g_btn_state[BTN_B].adc_max;
                        }
                    }
                    // g_btn_state[SLIDE_2_DIR_TRIG].capsense_status = 0;
                }
                else
                {
                    if (g_btn_state[SLIDE_1_DIR_TRIG].state_handle == 0)
                    {
                        // adc_res_index = BTN_A_ADC_INDEX;
                        g_btn_state[SLIDE_1_DIR_TRIG].adc_res_index = BTN_A_ADC_INDEX;
                        g_btn_state[SLIDE_1_DIR_TRIG].delta_D1 = BTN_A_DELTA_D1;
                        g_btn_state[SLIDE_1_DIR_TRIG].delta_D2 = BTN_A_DELTA_D2;
                        g_btn_state[SLIDE_1_DIR_TRIG].delta_D3 = BTN_A_DELTA_D3;
                        // if (g_btn_state[SLIDE_1_DIR_TRIG].state_handle == 0)
                        {
                            g_btn_state[SLIDE_1_DIR_TRIG].adc_res = g_btn_state[BTN_A].adc_res;
                            g_btn_state[SLIDE_1_DIR_TRIG].adc_min = g_btn_state[BTN_A].adc_min;
                            g_btn_state[SLIDE_1_DIR_TRIG].adc_max = g_btn_state[BTN_A].adc_max;
                        }
                    }
                    // g_btn_state[SLIDE_1_DIR_TRIG].capsense_status = 0;
                    // g_btn_state[SLIDE_2_DIR_TRIG].capsense_status = 1;
                }
                g_btn_state[SLIDE_1_DIR_TRIG].capsense_status = 1;
            }
            else
            {
                g_btn_state[SLIDE_1_DIR_TRIG].capsense_status = 0;
                // g_btn_state[SLIDE_2_DIR_TRIG].capsense_status = 0;
            }
            adc_res_index = g_btn_state[SLIDE_1_DIR_TRIG].adc_res_index;
            g_btn_state[SLIDE_1_DIR_TRIG].adc_res = ADC_0_measurement[adc_res_index];
            for (uint8_t i = 0; i < BTN_NUM; i++)
            {
                state_mahine_run(&g_btn_state[i]);
            }
        }

        // int res1 = g_btn_b_state.adc_res;
        // int res2 = g_btn_b_state.adc_min;

        //         int res3 = g_btn_a_state.adc_res;
        //         int res4 = g_btn_a_state.adc_min;

        // printf(":%d,%d,%d,%d\r\n", res1, res2,res3,res4);
    }
}