#pragma once

#include <stddef.h>
#include <stdint.h>
// #define USER_BOOTLOADER
#define LED_DEFAULT_REG_VAL 0x0
#define APP_MASK_EXCEL_DEF 0x460DB0A7
#define LIN_USART_X USART1
#define LIN_USART_ISR_VECT USART1_RXC_vect
#define LIN_TX_ID 0x22
#define LIN_NAD 0x61
#define LIN_TX_SET_DIR PC0_set_dir
#define LIN_TX_SET_LEVEL PC0_set_level
#define DUTY_MAX 0x4a

void target_soft_timer_init(void);
size_t USART0_Read(uint8_t * rDATA);
size_t USART0_Send(uint8_t * tDATA);
void soft_timer_touch_press_motor_task(void);
void lin_go_to_sleep(void);
