#ifndef __DAC_H__
#define __DAC_H__

#pragma once

#include <stdint.h>

#include "atmel_start.h"

void dac_timer_task(void);
void dac_run(void);
uint8_t dac_ret_status(void);
void dac_slide_run(void);
void dac_run_test(void);

#endif