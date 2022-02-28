#pragma once
#define LED_H_

#include <stdint.h>

typedef enum
{
    led_d6, //PWM1
    led_d8,
    led_d9,
    led_d7,
    led_d10,
    led_d11,
    led_d12,
    led_d13,
    led_d17,
    led_d14,
    led_d20,
    led_d18,
    led_d23,
    led_d22, //PWM1--END
    led_d21, //PWM2
    led_d19, //PWM3
    led_d15, //PWM4
    led_d16, //PWM5
} led_Dx_e;

void led_set_level(led_Dx_e Dx, uint8_t level);
void led_set_brightness(uint32_t reg_val);
void led_init(void);
