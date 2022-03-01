#include "ep40_cfg.h"
#include "led.h"
#include <atmel_start.h>
#include "touch.h"
#include "ep40_function_specification.h"
#include "lin.h"
#include "lin.h"
#include "lin_cfg.h"
#include "led.h"
#include "touch_press_motor.h"
#include "target.h"

#define UPDATE_CAP_PRESS_FLG(x)                          \
    uint8_t btn_index = x.btn_index;                     \
                                                         \
    if (btn_index > 64)                                  \
    {                                                    \
        return;                                          \
    }                                                    \
                                                         \
    static uint8_t press_status_last = 0;                \
    static uint8_t press_status = 0;                     \
                                                         \
    press_status_last = press_status;                    \
    if(g_btn_state[0].slide_trig_flg==btn_index)       \
    {                                                   \
        press_status = g_btn_state[0].state_handle;      \
    }else{                                              \
        press_status=0;                                 \
    }                                                   \
    if ((press_status == 1) && (press_status_last != 1)) \
    {                                                    \
        x.press_trig_flg = 1;                            \
        x.cap_trig_flg = 1;                              \
    }

typedef struct
{
    uint8_t status;
    uint16_t ticks;
    uint8_t btn_a_status_last;
    uint8_t btn_a_status;
    uint8_t btn_b_status_last;
    uint8_t btn_b_status;
    uint8_t slide_status_last;
    uint8_t slide_status;
    uint8_t slide_pos_first;
    uint8_t slide_pos;
    window_t window;
} window_obj_t;

typedef struct
{
    uint8_t cnt;
    uint8_t val;
    uint8_t satus;
} lin_signal_cnt_t;

DFGLS_signal_t g_DFGLS_signal;
LIN_RX_signal_t g_lin_rx_signal = {
    .backlight_brightness = 4,
};
lin_signal_cnt_t g_lin_signal_cnt[LIN_NUM_OF_SIGS];

void rear_led_ctrl(uint8_t status);
void window_lock_led_cb(uint8_t status);
void window_lock_signal_cb(void);
void backlight_cb(uint32_t duty);
void left_window_signal_out_cb(uint8_t signal);
void right_window_signal_out_cb(uint8_t signal);

btn_rear_t g_btn_rear = {
    .btn_index = REAR_BTN_INDEX,
    .rear_led_ctrl_cb = rear_led_ctrl,
};
window_lock_t g_window_lock = {
    .btn_index = WINDOW_LOCK_BTN_INDEX,
    .led_cb = window_lock_led_cb,
    .signal_cb = window_lock_signal_cb,
};
backlight_cb_t g_backlight_cb = backlight_cb;
window_obj_t g_left_window_obj = {
    .window = {
        .cb = left_window_signal_out_cb,
    },
};

window_obj_t g_right_window_obj = {
    .window = {
        .cb = right_window_signal_out_cb,
    },
};

void soft_timer_rear_btn_task(void)
{
    UPDATE_CAP_PRESS_FLG(g_btn_rear)
        uint8_t all_cap_status = 0;
    for (uint8_t i = 0; i < 6; i++)
    {
        if ((get_sensor_state(i) & 0x80) != 0)
        {
            all_cap_status = 1;
        }
    }
    for (uint8_t i = 0; i < 2; i++)
    {
        if (get_scroller_state(i) != 0)
        {
            all_cap_status = 1;
        }
    }

    g_btn_rear.all_cap_status = all_cap_status;

    btn_rear_tick_task();
}

void rear_led_ctrl(uint8_t status)
{
    if (status == 1)
    {
        led_set_level(led_d15, 1);
        led_set_level(led_d16, 0);
    }
    else
    {
        led_set_level(led_d15, 0);
        led_set_level(led_d16, 1);
    }
}

void window_lock_led_cb(uint8_t status)
{
    if (status == 1)
    {
        led_set_level(led_d19, 1);
        led_set_level(led_d21, 0);
    }
    else
    {
        led_set_level(led_d19, 0);
        led_set_level(led_d21, 1);
    }
}

/* 窗锁止定时器按键触摸检测任务 */
void soft_timer_window_lock_task(void)
{
    UPDATE_CAP_PRESS_FLG(g_window_lock)
        window_lock_task();
}

void window_lock_signal_cb(void)
{
    lin_signal_send_x_times(LI0_DFGLS_ControlWindowLockSw, 3, 1);
}
void soft_timer_lin_signal_update_task(void)
{
    if (l_u8_rd_LI0_DDCU_BDCS1_Backlight_brightness_fb() != 0)
    {
        g_lin_rx_signal.backlight_brightness = l_u8_rd_LI0_DDCU_BDCS1_Backlight_brightness_fb();
    }
    g_lin_rx_signal.backlight_status = l_bool_rd_LI0_DDCU_BDCS1_BacklightStatus();
    g_lin_rx_signal.ctrl_window_lock = l_bool_rd_LI0_DDCU_ControlWindowLockSwInd();
}
void backlight_cb(uint32_t duty)
{
    led_set_brightness(duty);
}


void ep40_lin_signal_update_while_task(void)
{
    for (uint8_t i = 0; i < LIN_NUM_OF_SIGS; i++)
    {
        switch (i)
        {
        case LI0_DFGLS_FLWindowControl:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_u8_wr_LI0_DFGLS_FLWindowControl(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_u8_wr_LI0_DFGLS_FLWindowControl(0);
            }
            break;

        case LI0_DFGLS_FRWindowControl:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_u8_wr_LI0_DFGLS_FRWindowControl(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_u8_wr_LI0_DFGLS_FRWindowControl(0);
            }
            break;

        case LI0_DFGLS_ControlWindowLockSw:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_bool_wr_LI0_DFGLS_ControlWindowLockSw(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_bool_wr_LI0_DFGLS_ControlWindowLockSw(0);
            }
            break;

        case LI0_DFGLS_ErrorSts:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_bool_wr_LI0_DFGLS_ErrorSts(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_bool_wr_LI0_DFGLS_ErrorSts(0);
            }
            break;

        case LI0_DFGLS_RLWindowControl:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_u8_wr_LI0_DFGLS_RLWindowControl(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_u8_wr_LI0_DFGLS_RLWindowControl(0);
            }
            break;

        case LI0_DFGLS_RRWindowControl:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_u8_wr_LI0_DFGLS_RRWindowControl(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_u8_wr_LI0_DFGLS_RRWindowControl(0);
            }
            break;

        case LI0_DFGLS_ButtonSoundRequest:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_bool_wr_LI0_DFGLS_ButtonSoundRequest(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_bool_wr_LI0_DFGLS_ButtonSoundRequest(0);
            }
            break;

        case LI0_DFGLS_Voltage_errorCode:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_bool_wr_LI0_DFGLS_Voltage_errorCode(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_bool_wr_LI0_DFGLS_Voltage_errorCode(0);
            }
            break;

        case LI0_DFGLS_Capsense_errorCode:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_bool_wr_LI0_DFGLS_Capsense_errorCode(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_bool_wr_LI0_DFGLS_Capsense_errorCode(0);
            }
            break;

        case LI0_DFGLS_Motor_errorCode:
            if (g_lin_signal_cnt[i].cnt > 0)
            {
                g_lin_signal_cnt[i].cnt--;
                l_bool_wr_LI0_DFGLS_Motor_errorCode(g_lin_signal_cnt[i].val);
            }
            else
            {
                l_bool_wr_LI0_DFGLS_Motor_errorCode(0);
            }
            break;

        default:
            break;
        }
    }
}

void lin_signal_send_x_times(l_signal_handle signal_handle, uint8_t cnt, uint8_t val)
{
    g_lin_signal_cnt[signal_handle].cnt = cnt;
    g_lin_signal_cnt[signal_handle].val = val;
}

void window_obj_var_clear(window_obj_t* handle)
{
    handle->status = 0;
    handle->ticks = 0;
    handle->btn_a_status_last = 0;
    handle->btn_a_status = 0;
    handle->btn_b_status_last = 0;
    handle->btn_b_status = 0;
    handle->slide_status_last = 0;
    handle->slide_status = 0;
    handle->slide_pos_first = 0;
    handle->slide_pos = 0;
    handle->window.status = 0;
    handle->window.long_flg = 0;
    handle->window.btn_b_cap_trig_flg = 0;
    handle->window.btn_a_cap_trig_flg = 0;
    handle->window.slide_1_to_2_flg = 0;
    handle->window.slide_2_to_1_flg = 0;
    handle->window.btn_a_status = 0;
    handle->window.btn_b_status = 0;
    handle->window.slide_status = 0;
    handle->window.signal = 0;
}

/* 左边窗提升信号输出回调函数 */
void left_window_signal_out_cb(uint8_t signal)
{
    uint8_t rear_status = g_btn_rear.status;

    if (rear_status == 0)
    {
        lin_signal_send_x_times(LI0_DFGLS_FLWindowControl, 3, signal);
    }
    else
    {
        lin_signal_send_x_times(LI0_DFGLS_RLWindowControl, 3, signal);
    }
}

/* 右边窗提升信号输出回调函数 */
void right_window_signal_out_cb(uint8_t signal)
{
    uint8_t rear_status = g_btn_rear.status;

    if (rear_status == 0)
    {
        lin_signal_send_x_times(LI0_DFGLS_FRWindowControl, 3, signal);
    }
    else
    {
        lin_signal_send_x_times(LI0_DFGLS_RRWindowControl, 3, signal);
    }
}
void window_task_prototype(window_obj_t* _window_obj_ptr, uint8_t btn_a_index, uint8_t btn_b_index)
{
    window_obj_t* window_obj_ptr = &g_left_window_obj;
    window_t* window_ptr = &g_left_window_obj.window;
    static uint8_t rear_status_last = 0;
    static uint8_t rear_status = 0;

    rear_status_last = rear_status;
    rear_status = g_btn_rear.status;

    /*************************************LEFT************************************************/

    if (rear_status_last != rear_status)
    {
        window_obj_var_clear(window_obj_ptr);
    }

    static uint8_t press_status_last_a = 0;
    static uint8_t press_status_a = 0;
    static uint8_t press_status_last_b = 0;
    static uint8_t press_status_b = 0;

    press_status_last_a = press_status_a;
    press_status_a = g_btn_state[btn_a_index].state_handle;

    press_status_last_b = press_status_b;
    press_status_b = g_btn_state[btn_b_index].state_handle;

    if (press_status_a == 1)
    {
        if (g_btn_state[btn_a_index].slide_trig_flg == 1)
        {
            window_ptr->btn_a_status = 1;
        }
        else {
            window_ptr->btn_a_status = 0;
        }
    }
    else
    {
        window_ptr->btn_a_status = 0;
    }

    if (press_status_b == 1)
    {
        if (g_btn_state[btn_b_index].slide_trig_flg == 1)
        {
            window_ptr->btn_b_status = 1;
        }
        else
        {
            window_ptr->btn_b_status = 0;
        }
    }
    else
    {
        window_ptr->btn_b_status = 0;
    }

    if (window_obj_ptr->status == 0)
    {
        if (press_status_a == 1)
        {
            if (g_btn_state[btn_a_index].slide_trig_flg == 1)
            {
                window_obj_ptr->ticks = 0;
                window_obj_ptr->status = 2;
            }
        }
    }

    if (window_obj_ptr->status == 2)
    {
        window_obj_ptr->ticks++;
        window_ptr->long_flg = 1;
        window_ptr->btn_a_cap_trig_flg = 1;
        window_obj_ptr->status = 0;

#if 0
        if (window_obj_ptr->btn_a_status == 0)
        {
            window_obj_ptr->status = 0;
            window_ptr->long_flg = 0;
            window_ptr->btn_a_cap_trig_flg = 1;
        }
#endif
    }

    if (window_obj_ptr->status == 0)
    {
        if (press_status_b == 1)
        {
            if (g_btn_state[btn_b_index].slide_trig_flg == 1)
            {
                window_obj_ptr->ticks = 0;
                window_obj_ptr->status = 3;
            }
        }
    }

    if (window_obj_ptr->status == 3)
    {
        window_obj_ptr->ticks++;
        window_ptr->long_flg = 1;
        window_ptr->btn_b_cap_trig_flg = 1;
        window_obj_ptr->status = 0;

#if 0
        if (window_obj_ptr->btn_b_status == 0)
        {
            window_obj_ptr->status = 0;
            window_ptr->long_flg = 0;
            window_ptr->btn_b_cap_trig_flg = 1;
        }
#endif
    }

    window_obj_ptr->slide_status_last = window_obj_ptr->slide_status;
    if (g_btn_state[SLIDE_1_DIR_TRIG].slide_trig_flg == 1)
    {
        window_obj_ptr->slide_status = g_btn_state[SLIDE_1_DIR_TRIG].state_handle;
    }
    else {
        window_obj_ptr->slide_status = 0;
    }
    window_ptr->slide_status = window_obj_ptr->slide_status;

    window_obj_ptr->slide_pos = get_scroller_position(0);

    if (window_obj_ptr->status == 0)
    {
        if (window_obj_ptr->slide_status == 1)
        {
            window_obj_ptr->ticks = 0;
            window_obj_ptr->status = 4;
            window_obj_ptr->slide_pos_first = window_obj_ptr->slide_pos;
        }
    }
    else if (window_obj_ptr->status == 4)
    {
        if (get_scroller_state(0) == 0)
        {
            window_obj_ptr->status = 0;
        }
        if (window_obj_ptr->slide_pos_first < 100)
        {
            if (window_obj_ptr->slide_pos > (window_obj_ptr->slide_pos_first + 100))
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_1_to_2_flg = 1;
                window_obj_ptr->status = 5;
            }
        }
        else if (window_obj_ptr->slide_pos_first > 150)
        {
            if ((window_obj_ptr->slide_pos + 100) < window_obj_ptr->slide_pos_first)
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_2_to_1_flg = 1;
                window_obj_ptr->status = 5;
            }
        }
        else
        {
            if (window_obj_ptr->slide_pos > (window_obj_ptr->slide_pos_first + 100))
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_1_to_2_flg = 1;
                window_obj_ptr->status = 5;
            }
            else if ((window_obj_ptr->slide_pos + 100) < window_obj_ptr->slide_pos_first)
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_2_to_1_flg = 1;
                window_obj_ptr->status = 5;
            }
        }
    }
    else if (window_obj_ptr->status == 5)
    {
        if (window_obj_ptr->slide_status == 0)
        {
            window_obj_ptr->status = 0;
        }
    }
    window_task(window_ptr);

    /*************************************RIGHT************************************************/
    window_obj_ptr = &g_right_window_obj;
    window_ptr = &g_right_window_obj.window;
    if (rear_status_last != rear_status)
    {
        window_obj_var_clear(window_obj_ptr);
    }

    if (press_status_a == 1)
    {
        if (g_btn_state[btn_a_index].slide_trig_flg == 0)
        {
            window_ptr->btn_a_status = 1;
        }
        else {
            window_ptr->btn_a_status = 0;
        }
    }
    else
    {
        window_ptr->btn_a_status = 0;
    }

    if (press_status_b == 1)
    {
        if (g_btn_state[btn_b_index].slide_trig_flg == 0)
        {
            window_ptr->btn_b_status = 1;
        }
        else
        {
            window_ptr->btn_b_status = 0;
        }
    }
    else
    {
        window_ptr->btn_b_status = 0;
    }

    if (window_obj_ptr->status == 0)
    {
        if (press_status_a == 1)
        {
            if (g_btn_state[btn_a_index].slide_trig_flg == 0)
            {
                window_obj_ptr->ticks = 0;
                window_obj_ptr->status = 2;
            }
        }
    }

    if (window_obj_ptr->status == 2)
    {
        window_obj_ptr->ticks++;
        window_ptr->long_flg = 1;
        window_ptr->btn_a_cap_trig_flg = 1;
        window_obj_ptr->status = 0;

#if 0
        if (window_obj_ptr->btn_a_status == 0)
        {
            window_obj_ptr->status = 0;
            window_ptr->long_flg = 0;
            window_ptr->btn_a_cap_trig_flg = 1;
        }
#endif
    }

    if (window_obj_ptr->status == 0)
    {
        if (press_status_b == 1)
        {
            if (g_btn_state[btn_b_index].slide_trig_flg == 0)
            {
                window_obj_ptr->ticks = 0;
                window_obj_ptr->status = 3;
            }
        }
    }

    if (window_obj_ptr->status == 3)
    {
        window_obj_ptr->ticks++;
        window_ptr->long_flg = 1;
        window_ptr->btn_b_cap_trig_flg = 1;
        window_obj_ptr->status = 0;

#if 0
        if (window_obj_ptr->btn_b_status == 0)
        {
            window_obj_ptr->status = 0;
            window_ptr->long_flg = 0;
            window_ptr->btn_b_cap_trig_flg = 1;
        }
#endif
    }

    window_obj_ptr->slide_status_last = window_obj_ptr->slide_status;
    if (g_btn_state[SLIDE_1_DIR_TRIG].slide_trig_flg == 0)
    {
        window_obj_ptr->slide_status = g_btn_state[SLIDE_1_DIR_TRIG].state_handle;
    }
    else {
        window_obj_ptr->slide_status = 0;
    }
    window_ptr->slide_status = window_obj_ptr->slide_status;

    window_obj_ptr->slide_pos = get_scroller_position(1);

    if (window_obj_ptr->status == 0)
    {
        if (window_obj_ptr->slide_status == 1)
        {
            window_obj_ptr->ticks = 0;
            window_obj_ptr->status = 4;
            window_obj_ptr->slide_pos_first = window_obj_ptr->slide_pos;
        }
    }
    else if (window_obj_ptr->status == 4)
    {
        if (get_scroller_state(1) == 0)
        {
            window_obj_ptr->status = 0;
        }
        if (window_obj_ptr->slide_pos_first < 100)
        {
            if (window_obj_ptr->slide_pos > (window_obj_ptr->slide_pos_first + 100))
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_1_to_2_flg = 1;
                window_obj_ptr->status = 5;
            }
        }
        else if (window_obj_ptr->slide_pos_first > 150)
        {
            if ((window_obj_ptr->slide_pos + 100) < window_obj_ptr->slide_pos_first)
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_2_to_1_flg = 1;
                window_obj_ptr->status = 5;
            }
        }
        else
        {
            if (window_obj_ptr->slide_pos > (window_obj_ptr->slide_pos_first + 100))
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_1_to_2_flg = 1;
                window_obj_ptr->status = 5;
            }
            else if ((window_obj_ptr->slide_pos + 100) < window_obj_ptr->slide_pos_first)
            {
                window_ptr->long_flg = 0;
                window_ptr->slide_2_to_1_flg = 1;
                window_obj_ptr->status = 5;
            }
        }
#if 0
        if (window_obj_ptr->slide_status == 0)
        {
            window_obj_ptr->status = 0;
            if (window_obj_ptr->slide_pos_first < 100)
            {
                if (window_obj_ptr->slide_pos > 150)
                {
                    window_ptr->long_flg = 0;
                    window_ptr->slide_1_to_2_flg = 1;
                }
            }
            else if (window_obj_ptr->slide_pos_first > 150)
            {
                if (window_obj_ptr->slide_pos < 100)
                {
                    window_ptr->long_flg = 0;
                    window_ptr->slide_2_to_1_flg = 1;
                }
            }
        }
        else
        {
            window_obj_ptr->ticks++;
            if (window_obj_ptr->slide_status != 0)
            {
                if (window_obj_ptr->slide_pos_first < 100)
                {
                    if (window_obj_ptr->slide_pos > 150)
                    {
                        if (window_obj_ptr->ticks >= (LONG_PRESS_DELTA / WINDOW_TICK_UNIT_MS))
                        {
                            window_ptr->slide_1_to_2_flg = 1;
                            window_obj_ptr->status = 5;
                            window_ptr->long_flg = 1;
                        }
                        else
                        {
                            window_ptr->slide_1_to_2_flg = 1;
                            window_ptr->long_flg = 1;
                        }
                    }
                }
                else if (window_obj_ptr->slide_pos_first > 150)
                {
                    if (window_obj_ptr->slide_pos < 100)
                    {
                        if (window_obj_ptr->ticks >= (LONG_PRESS_DELTA / WINDOW_TICK_UNIT_MS))
                        {
                            window_obj_ptr->status = 5;
                            window_ptr->long_flg = 1;
                            window_ptr->slide_2_to_1_flg = 1;
                        }
                        else
                        {
                            window_ptr->long_flg = 1;
                            window_ptr->slide_2_to_1_flg = 1;
                        }
                    }
                }
            }
        }
#endif
    }
    else if (window_obj_ptr->status == 5)
    {
        if (window_obj_ptr->slide_status == 0)
        {
            window_obj_ptr->status = 0;
        }
    }
    window_task(window_ptr);
}

/* 窗提升滑条按键软件定时器任务 */
void soft_timer_window_task(void)
{
    window_task_prototype(&g_right_window_obj, BTN_A, BTN_B);
}
