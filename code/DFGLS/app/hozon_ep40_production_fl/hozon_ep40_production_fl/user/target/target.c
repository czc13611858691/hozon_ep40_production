#include "target.h"
#include "soft_timer.h"
#include "led.h"
#include "ep40_cfg.h"
#include "touch.h"
#include <stdio.h>

void target_soft_timer_init(void)
{
    soft_timer_create(20, soft_timer_rear_btn_task);
	soft_timer_create(20, soft_timer_window_lock_task);
	soft_timer_create(20, soft_timer_window_task);
	soft_timer_create(10, soft_timer_lin_signal_update_task);
	soft_timer_create(100, backlight_task);
}

size_t USART0_Read(uint8_t * rDATA)
{
	*rDATA = LIN_USART_X.RXDATAL;
	return 0;
}

size_t USART0_Send(uint8_t * tDATA)
{
	LIN_USART_X.TXDATAL = *tDATA;
	return 0;
}

void motor_shake_cb(void)
{
}

void soft_timer_touch_press_motor_task(void)
{

}

void __attribute__((optimize("O0"))) lin_go_to_sleep(void)
{
    LIN_EN_set_level(0);

    Disable_global_interrupt();
    LIN_USART_X.CTRLA &= ~(1 << USART_ABEIE_bp | 1 << USART_RXCIE_bp);
    LIN_USART_X.CTRLB &= ~(1 << USART_TXEN_bp | USART_RXMODE_LINAUTO_gc | 1 << USART_RXEN_bp);

    LIN_TX_SET_DIR(PORT_DIR_OUT);
    LIN_TX_SET_LEVEL(false);
}
