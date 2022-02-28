#ifndef __function_specification_H_
#define __function_specification_H_
#include "boot_config.h"

void lin_go_to_sleep(void) __attribute__((optimize("O0")));
void ERASE_FLASH(void);
void UPDATE_FALSH(void);
void VALIDATE_PROGRAM(void);
//uint32_t GENERIC_ALGORITHM(uint32_t wSeed, uint32_t MASK);
void READ_DID_DATA(void);
void WRITE_DID_DATA(void);
void CALCULATE_KEY(void);
void ECU_RESET(void);
#endif