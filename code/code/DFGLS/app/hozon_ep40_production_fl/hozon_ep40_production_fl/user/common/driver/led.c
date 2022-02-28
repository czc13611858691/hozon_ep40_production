#include "led.h"
#include <atmel_start.h>
#include "target.h"

enum {
	LED_PWM1_REG_VAL,
	LED_PWM2_REG_VAL,
	LED_PWM3_REG_VAL,
	LED_PWM4_REG_VAL,
	LED_PWM5_REG_VAL,
};

uint8_t g_led_switch_status[5] = {
	1,1,0,0,1,
};

uint32_t g_led_pwm_val = LED_DEFAULT_REG_VAL;

static void led_updata_timer_val(void);

static void led_updata_timer_val(void)
{
	TCA0.SPLIT.LCMP2 = g_led_switch_status[0] * g_led_pwm_val;
	TCA0.SPLIT.HCMP0 = g_led_switch_status[1] * g_led_pwm_val;
	TCA0.SPLIT.HCMP1 = g_led_switch_status[2] * g_led_pwm_val;
	TCA0.SPLIT.LCMP0 = g_led_switch_status[3] * g_led_pwm_val;
	TCA0.SPLIT.LCMP1 = g_led_switch_status[4] * g_led_pwm_val;
}

void led_set_level(led_Dx_e Dx, uint8_t level)
{
	switch (Dx)
	{
	case led_d21:
		g_led_switch_status[1] = level;
		break;
	case led_d19:
		g_led_switch_status[2] = level;
		break;
	case led_d15:
		g_led_switch_status[3] = level;
		break;
	case led_d16:
		g_led_switch_status[4] = level;
		break;

	default:
		g_led_switch_status[0] = level;
		break;
	}
	led_updata_timer_val();
}

void led_set_brightness(uint32_t reg_val)
{
	g_led_pwm_val=reg_val;
	led_updata_timer_val();
}

void led_init(void)
{
	led_updata_timer_val();
}