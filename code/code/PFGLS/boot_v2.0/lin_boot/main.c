#include <atmel_start.h>
#include "lin_driver.h"
#include "lin1.h"
#include "lin_common_api.h"
#include "lin_commontl_api.h"
#include "function_specification.h"
#include "ccp.h"
#include "rstctrl.h"
#include <atomic.h>
#include "fuse.h"
/*
*  bootloader 仅支持诊断功能中的刷新会话02, LIN通信相关功能将被移除
*/

int main(void)
{
	/* 外部刷新请求 */
	uint8_t programming_request_flg;

	/* 应用程序是否有效 */
	uint8_t application_valid_flg;

	DISABLE_INTERRUPTS();
	ccp_write_io((void*)&(CPUINT.CTRLA), (CPUINT_IVSEL_bm | CPUINT.CTRLA));
	atmel_start_init();

	programming_request_flg = FLASH_0_read_eeprom_byte(PROGRAM_REQUEST_EEPROM_POS); //读取eeprom的标志位
	application_valid_flg = FLASH_0_read_eeprom_byte(APPLICATION_VALID_EEPROM_POS); //读取eeprom的标志位

	#if 1
	/* 如果检测到外部刷写请求标志位置位，保持在bootloader程序中，同时设置会话模式为编程模式*/
	if (programming_request_flg == 1)
	{
		FLASH_0_write_eeprom_byte(PROGRAM_REQUEST_EEPROM_POS, 0);

		g_sessionStatus = SESSION_PROGRAM;
	}
	else /* 如果没有外部刷写请求:若应用程序合理，则跳转application程序空间;若是应用程序不合理，则保持在bootloader程序中，同时设置会话模式为默认会话01*/
	{
		/* 检查应用程序是否有效标志位，如果应用程序有??*/
		if (application_valid_flg == 1)
		{
			ccp_write_io((void*)&(CPUINT.CTRLA), 0);
			asm volatile("JMP 0x6400"::);
		}
		else {
			g_sessionStatus = SESSION_DEFAULT;
		}
	}
	#endif

	g_noDefaultSessionTicks = 5000;
	/* 初始化外设，会开启中断的外设 */
	USART_0_initialization();
	TIMER_0_initialization();
	ENABLE_INTERRUPTS();

	l_sys_init();
	l_ifc_init(LI0);
	ld_init(LI0);

	READ_DID_DATA();

	while (1)
	{
		ERASE_FLASH();		//等待擦除任务
		UPDATE_FALSH(); 	//等待刷写任务
		VALIDATE_PROGRAM();
		WRITE_DID_DATA();
		CALCULATE_KEY();
		ECU_RESET();
	}
}


