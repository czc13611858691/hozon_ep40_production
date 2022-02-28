/*
 * app_config.h
 *
 * Created: 2021/10/27 16:12:10
 *  Author: JasonZhu
 */
#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_
#include "stdint.h"

#define BOOT_MASK 0xB10C20AC
#define SECURITY_SEED_LEN 4
#define FLASH_PAGE_SIZE 512U
#define UPDATE_MEMORY_ADDR (0x6400)

#define SESSION_DEFAULT     1U
#define SESSION_PROGRAM     2U
#define SESSION_EXTEND      3U

#define SECURITY_ACCESS_LOCK        0U
#define SECURITY_ACCESS_LEVEL_1     3U
#define SECURITY_ACCESS_LEVEL_FBL   11U

#define PROGRAM_REQUEST_EEPROM_POS 0
#define APPLICATION_VALID_EEPROM_POS 1
#define PRAOGRAM_CHECK_EEPROM_POS 2

/* 全局变量 一共 */




/**内存擦除\刷写\检验操作用到的变量 1 **/
extern uint32_t page_addr;              

/**擦除程序 1 **/
extern uint8_t erase_flag;

/**刷写程序 7 **/
extern uint8_t flash_flag;
extern uint32_t bin_size_cnt;
extern uint16_t update_cnt;
extern uint8_t program_buffer[512];
extern uint8_t history_blcok_count;
extern uint32_t update_mem_size;
extern uint32_t update_mem_addr;
/***********************/

//1
extern uint8_t download_request_flag; 	//请求下载标志位,在传输数据时判断

/**检查刷写程序的有效性 3 **/
extern uint8_t  check_program_flag;		//程序有效性判断：0：位请求判断 1：请求判断 2：程序有效 3：程序无效
extern uint32_t check_program_size;		//检查程序空间大小
extern uint16_t crc16_code; 			//CRC16运算结果
/***********************/

//1
extern uint8_t reset_flag;

//1
extern uint8_t ecu_rst_flg;

//3
volatile extern uint16_t g_noDefaultSessionTicks;
volatile extern uint8_t g_sessionStatus;
volatile extern uint8_t g_security_access;

//4
volatile extern uint8_t g_request_seed_flg_LEVEL_FBL;
volatile extern uint32_t SECURITY_ACCESS_KEY;
volatile extern uint32_t SECURITY_ACCESS_SEED;
volatile extern uint16_t g_soft_timer_ticks;

typedef struct {
	uint8_t start;
	uint8_t len;
}eeprom_did_data_t;
extern const eeprom_did_data_t VINDataIdentifier_eeprom;
extern uint8_t gVINDataIdentifier[17];
extern uint8_t gVINDataIdentifier_update_flg;
extern const eeprom_did_data_t TesterSerialNumberDataIdentifier_eeprom;
extern uint8_t gTesterSerialNumberDataIdentifier[10];
extern uint8_t gTesterSerialNumberDataIdentifier_update_flg;
extern const eeprom_did_data_t ProgrammingDataDataIdentifier_eeprom;
extern uint8_t gProgrammingDataDataIdentifier[4];
extern uint8_t gProgrammingDataDataIdentifier_update_flg;
extern const eeprom_did_data_t ECUInstallationDateDataIdentifier_eeprom;
extern uint8_t gECUInstallationDateDataIdentifier[4];
extern uint8_t gECUInstallationDateDataIdentifier_update_flg;

#endif /* APP_CONFIG_H_ */
