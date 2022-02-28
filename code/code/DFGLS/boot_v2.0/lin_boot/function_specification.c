#include "function_specification.h"
#include "atmel_start_pins.h"
#include "lin.h"
#include "lin_commontl_api.h"
#include "rstctrl.h"

const eeprom_did_data_t VINDataIdentifier_eeprom = {
	.start = 10,
	.len = 17,
};

uint8_t gVINDataIdentifier[17] = { 0 };
uint8_t gVINDataIdentifier_update_flg = 0;

const eeprom_did_data_t TesterSerialNumberDataIdentifier_eeprom = {
	.start = 30,
	.len = 10,
};

uint8_t gTesterSerialNumberDataIdentifier[10] = { 0 };
uint8_t gTesterSerialNumberDataIdentifier_update_flg = 0;

const eeprom_did_data_t ProgrammingDataDataIdentifier_eeprom = {
	.start = 40,
	.len = 4,
};

uint8_t gProgrammingDataDataIdentifier[4] = { 0 };
uint8_t gProgrammingDataDataIdentifier_update_flg = 0;

const eeprom_did_data_t ECUInstallationDateDataIdentifier_eeprom = {
	.start = 50,
	.len = 4,
};

uint8_t gECUInstallationDateDataIdentifier[4] = { 0 };
uint8_t gECUInstallationDateDataIdentifier_update_flg = 0;


uint8_t history_blcok_count = 0;
uint16_t update_cnt = 0;
uint32_t bin_size_cnt = 0;
uint32_t update_mem_size = 0;
uint32_t update_mem_addr = 0;
uint8_t download_request_flag = 0;
uint8_t erase_flag = 0;
uint8_t flash_flag = 0;
uint8_t reset_flag = 0;
uint8_t check_program_flag = 0u;
uint32_t check_program_size = 0;
uint16_t crc16_code;
uint32_t page_addr = UPDATE_MEMORY_ADDR;
uint8_t program_buffer[FLASH_PAGE_SIZE] = { 0 };
uint8_t ecu_rst_flg = 0;
volatile uint32_t SECURITY_ACCESS_SEED = 0;
volatile uint32_t SECURITY_ACCESS_KEY = 0;
volatile uint8_t g_request_seed_flg_LEVEL_FBL = 0;
volatile uint16_t g_noDefaultSessionTicks;

  /* 会话模式:上电默认会话模式(0x01) */
volatile uint8_t g_sessionStatus = SESSION_DEFAULT;

/* 安全等级:默认安全等级为锁定状态 */
volatile uint8_t g_security_access = SECURITY_ACCESS_LOCK;

/**
 * @brief LIN休眠功能，如果使用了框架，需要放到LIN_DRV_GoToSleepMode()函数内执行
 */
void lin_go_to_sleep(void) 
{
    uint16_t sleep_cnt;
    /* 使能脚设置低电平 */
    LIN_EN_set_level(0);

    /* 关闭usart模块 */
    Disable_global_interrupt();
    USART4.CTRLA &= ~(1 << USART_ABEIE_bp | 1 << USART_RXCIE_bp);
    USART4.CTRLB &= ~(1 << USART_TXEN_bp | USART_RXMODE_LINAUTO_gc | 1 << USART_RXEN_bp);

    /* LIN TX设置低电平 */
    PE0_set_dir(PORT_DIR_OUT);
    PE0_set_level(false);

	for(sleep_cnt = 0; sleep_cnt < 10000; sleep_cnt++);
	for(sleep_cnt = 0; sleep_cnt < 10000; sleep_cnt++);
	
	USART_0_init();
	
	LIN_EN_set_level(1);
	Enable_global_interrupt();
}

/**
 * @brief 擦除falsh
 */
void ERASE_FLASH(void)
{
    if (erase_flag == 1)
    {
        erase_flag = 0;
        //假擦除,直接将APPLICATION_VALID有效位置0
        FLASH_0_write_eeprom_byte(APPLICATION_VALID_EEPROM_POS, 0);
    }
}

/**
 * @brief 刷写flash
 */
void UPDATE_FALSH(void)
{
    uint16_t i;
    if (flash_flag == 1)
    {
        FLASH_0_write_flash_page(page_addr, program_buffer);
        page_addr = page_addr + FLASH_PAGE_SIZE;
        flash_flag = 0;
    }
    else if (flash_flag == 2u)
    {
        for (i = update_cnt;i < FLASH_PAGE_SIZE;i++) { program_buffer[i] = 0xff;}
        FLASH_0_write_flash_page(page_addr, program_buffer);
        flash_flag = 0;
        update_cnt = 0;
        page_addr = UPDATE_MEMORY_ADDR; //page_addr 回到初始位置
    }
}

//CRC运算表，合众企标 Q/THZ E8-22-2021 附录A
const uint16_t crctab[256] = {
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};


/**
 * @brief 检查程序可靠性
 */
void VALIDATE_PROGRAM(void)
{
    uint32_t i;
    uint8_t tmp_crc;
    uint16_t crc_seed;
    //执行内存校验,由UDS置写1u
    if (check_program_flag == 1u)
    {
        page_addr = UPDATE_MEMORY_ADDR;
        crc_seed = 0xffff;
        while (check_program_size >= 512)
        {
            for (i = 0;i < FLASH_PAGE_SIZE;i++)
            {
                tmp_crc = (crc_seed >> 8) ^ FLASH_0_read_flash_byte(page_addr + i);
                crc_seed = (crc_seed << 8) ^ crctab[tmp_crc];
            }
            page_addr += FLASH_PAGE_SIZE;
            check_program_size -= FLASH_PAGE_SIZE;
        }
        if (check_program_size > 0)
        {
            for (i = 0;i < check_program_size;i++)
            {
                tmp_crc = (crc_seed >> 8) ^ FLASH_0_read_flash_byte(page_addr + i);
                crc_seed = (crc_seed << 8) ^ crctab[tmp_crc];
            }
        }
        page_addr = UPDATE_MEMORY_ADDR;
        if ((crc_seed == crc16_code) && (crc16_code != 0u))
        {
            //校验正确，并APPLICATION_VALID置1
            check_program_flag = 2u;
            FLASH_0_write_eeprom_byte(APPLICATION_VALID_EEPROM_POS, 1);
        }
        else
        {
            //校验失败
            check_program_flag = 3u;
        }
    }
}

/**
 * @brief 复位MCU
 */
void ECU_RESET(void)
{
    if (ecu_rst_flg == 1)
    {
        /* 清零编程请求EEPROM变量 */
        FLASH_0_write_eeprom_byte(PROGRAM_REQUEST_EEPROM_POS, 0);
        while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm | NVMCTRL_FBUSY_bm))
            ;

        /* 发送完肯定响应信息，才能复位*/
        if (ld_raw_tx_status(0) == LD_QUEUE_EMPTY)
        {
            RSTCTRL_reset();
            
            // if (LIN_DRV_GetCurrentNodeState(0) == LIN_NODE_STATE_IDLE)
            // {
                
            // }
        }
    }
}
/**
 * @brief 密钥生成算法，合众企标 《合众EP40项目27服务安全访问算法》
 */
uint32_t GENERIC_ALGORITHM(uint32_t wSeed, uint32_t MASK)
{
    uint32_t iterations;
    uint32_t wLastSeed;
    uint32_t wTemp;
    uint32_t wLSBit;
    uint32_t wTop31Bits;
    uint32_t jj, SB1, SB2, SB3;
    uint16_t temp;
    wLastSeed = wSeed;
    temp = (uint16_t)((MASK & 0x00000800) >> 10) | ((MASK & 0x00200000) >> 21);
    if (temp == 0)
    {
        wTemp = (uint32_t)((wSeed | 0x00ff0000) >> 16);
    }
    else if (temp == 1)
    {
        wTemp = (uint32_t)((wSeed | 0xff000000) >> 24);
    }
    else if (temp == 2)
    {
        wTemp = (uint32_t)((wSeed | 0x0000ff00) >> 8);
    }
    else
    {
        wTemp = (uint32_t)(wSeed | 0x000000ff);
    }
    SB1 = (uint32_t)((MASK & 0x000003FC) >> 2);
    SB2 = (uint32_t)(((MASK & 0x7F800000) >> 23) ^ 0xA5);
    SB3 = (uint32_t)(((MASK & 0x001FE000) >> 13) ^ 0x5A);
    iterations = (uint32_t)(((wTemp | SB1) ^ SB2) + SB3);
    for (jj = 0; jj < iterations; jj++)
    {
        wTemp = ((wLastSeed ^ 0x40000000) / 0x40000000) ^ ((wLastSeed & 0x01000000) / 0x01000000)
            ^ ((wLastSeed & 0x1000) / 0x1000) ^ ((wLastSeed & 0x04) / 0x04);
        wLSBit = (wTemp ^ 0x00000001);
        wLastSeed = (uint32_t)(wLastSeed << 1);
        wTop31Bits = (uint32_t)(wLastSeed ^ 0xFFFFFFFE);
        wLastSeed = (uint32_t)(wTop31Bits | wLSBit);
    }
    if (MASK & 0x00000001)
    {
        wTop31Bits = ((wLastSeed & 0x00FF0000) >> 16) | ((wLastSeed ^ 0xFF000000) >> 8) | ((wLastSeed
            ^ 0x000000FF) << 8) | ((wLastSeed ^ 0x0000FF00) << 16);
    }
    else
        wTop31Bits = wLastSeed;
    wTop31Bits = wTop31Bits ^ MASK;
    return(wTop31Bits);
}

void READ_DID_DATA(void)
{
    /* 从EEPROM中读取数据(DID信息)*/
	for (uint8_t i = 0;i < VINDataIdentifier_eeprom.len;i++)
	{
		gVINDataIdentifier[i] = FLASH_0_read_eeprom_byte(VINDataIdentifier_eeprom.start + i);
	}
	for (uint8_t i = 0;i < TesterSerialNumberDataIdentifier_eeprom.len;i++)
	{
		gTesterSerialNumberDataIdentifier[i] = FLASH_0_read_eeprom_byte(TesterSerialNumberDataIdentifier_eeprom.start + i);
	}
	for (uint8_t i = 0;i < ProgrammingDataDataIdentifier_eeprom.len;i++)
	{
		gProgrammingDataDataIdentifier[i] = FLASH_0_read_eeprom_byte(ProgrammingDataDataIdentifier_eeprom.start + i);
	}
	for (uint8_t i = 0;i < ECUInstallationDateDataIdentifier_eeprom.len;i++)
	{
		gECUInstallationDateDataIdentifier[i] = FLASH_0_read_eeprom_byte(ECUInstallationDateDataIdentifier_eeprom.start + i);
	}
}

void WRITE_DID_DATA(void)
{
    /****************存储DID********************/
    if (gVINDataIdentifier_update_flg == 1)
    {
        gVINDataIdentifier_update_flg = 0;
        for (uint8_t i = 0;i < VINDataIdentifier_eeprom.len;i++)
        {
            FLASH_0_write_eeprom_byte(VINDataIdentifier_eeprom.start + i, gVINDataIdentifier[i]);
        }
    }

    if (gTesterSerialNumberDataIdentifier_update_flg == 1)
    {
        gTesterSerialNumberDataIdentifier_update_flg = 0;
        for (uint8_t i = 0;i < TesterSerialNumberDataIdentifier_eeprom.len;i++)
        {
            FLASH_0_write_eeprom_byte(TesterSerialNumberDataIdentifier_eeprom.start + i, gTesterSerialNumberDataIdentifier[i]);
        }
    }

    if (gProgrammingDataDataIdentifier_update_flg == 1)
    {
        gProgrammingDataDataIdentifier_update_flg = 0;

        for (uint8_t i = 0;i < ProgrammingDataDataIdentifier_eeprom.len;i++)
        {
            FLASH_0_write_eeprom_byte(ProgrammingDataDataIdentifier_eeprom.start + i, gProgrammingDataDataIdentifier[i]);
        }
    }

    if (gECUInstallationDateDataIdentifier_update_flg == 1)
    {
        gECUInstallationDateDataIdentifier_update_flg = 0;
        for (uint8_t i = 0;i < ECUInstallationDateDataIdentifier_eeprom.len;i++)
        {
            FLASH_0_write_eeprom_byte(ECUInstallationDateDataIdentifier_eeprom.start + i, gECUInstallationDateDataIdentifier[i]);
        }
    }
    /**************************************************/
}

void CALCULATE_KEY(void)
{
    //获取到种子后立刻进行KEY的计算	
    if(g_request_seed_flg_LEVEL_FBL == 1)
    {
        SECURITY_ACCESS_KEY = GENERIC_ALGORITHM(SECURITY_ACCESS_SEED,0xB10C20AC);
        g_request_seed_flg_LEVEL_FBL = 2;
    }
}