#pragma once

#include <stddef.h>
#include <stdint.h>
// #define USER_BOOTLOADER
#define LED_DEFAULT_REG_VAL 0x0
#define APP_MASK_EXCEL_DEF 0x9C07E065
#define LIN_USART_X USART4
#define LIN_USART_ISR_VECT USART4_RXC_vect
#define ADC_RES_0_CHANNEL ADC_MUXPOS_AIN10_gc
#define ADC_RES_1_CHANNEL ADC_MUXPOS_AIN11_gc
#define LIN_TX_ID 0x21
#define LIN_NAD 0x50
#define PRESS_DELTA_D1 2
#define PRESS_DELTA_D2 2
#define PRESS_DELTA_D3 2
#define LIN_TX_SET_DIR PE0_set_dir
#define LIN_TX_SET_LEVEL PE0_set_level
#define DUTY_MAX 0x4a

#define REAR_BTN_INDEX 0
#define WINDOW_LOCK_BTN_INDEX 1

void target_soft_timer_init(void);
size_t USART0_Read(uint8_t * rDATA);
size_t USART0_Send(uint8_t * tDATA);
void lin_go_to_sleep(void);