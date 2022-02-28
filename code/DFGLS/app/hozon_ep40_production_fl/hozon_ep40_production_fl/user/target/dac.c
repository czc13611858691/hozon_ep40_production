#include "atmel_start.h"
#include "dac.h"
#include "soft_timer.h"

/* 为dac输出单独开一个定时器中断(为了调手感)，时间为0.5ms */
// 512,757,944,1019,969,813,591,362,180,86,98,209,385,581,750,851,866,795,662,502,356,257,227,266,360,482,600,686,722,705,644,559,473,408,376,381,416,467,519,558,576,573,562,540,515,493,478,474,480,492,506,517,524,

//    495,    493,    492,    490,    488,    486,    485,    482,    479,    474,    469,    464,    457,    448,    438,    424,    404,    378,    344,    307,    267,    223,    180,    138,    98,    65,    39,    21,    12,    11,    17,    29,    46,    66,    89,    111,    133,    154,    173,    192,    209,    226,    243,    260,    276,    292,    311,    330,    349,    369,    388,    406,    422,    439,    453,    467,    478,    489,    498,    506,    516,    526,    536,    545,    555,    565,    575,    586,    596,
const uint16_t SIN_DATA[] = {
495,    493,    492,    490,    488,    486,    485,    482,    479,    474,    469,    464,    457,    448,    438,    424,    404,    378,    344,    307,    267,    223,    180,    138,    98,    65,    39,    21,    12,    11,    17,    29,    46,    66,    89,    111,    133,    154,    173,    192,    209,    226,    243,    260,    276,    292,    311,    330,    349,    369,    388,    406,    422,    439,    453,    467,    478,    489,    498,    506,    516,    526,    536,    545,    555,    565,    575,    586,    596,
};
const uint16_t SIN_DATA_RESERVE[] = {
   596, 586, 575, 565, 555, 545, 536, 526, 516, 506, 498, 489, 478, 467, 453, 439, 422, 406, 388, 369, 349, 330, 311, 292, 276, 260, 243, 226, 209, 192, 173, 154, 133, 111, 89, 66, 46, 29, 17, 11, 12, 21, 39, 65, 98, 138, 180, 223, 267, 307, 344, 378, 404, 424, 438, 448, 457, 464, 469, 474, 479, 482, 485, 486, 488, 490, 492, 493, 495};
const uint16_t SIN_DATA_SLIDE[] = {
512,757,944,1019,969,813,591,362,180,86,98,209,385,581,750,851,866,795,662,502
};

uint16_t g_sin_data_index = 0;
uint8_t g_sin_data_out_flg = 0;
uint16_t g_slide_sin_data_index = 0;
uint8_t g_slide_sin_data_out_flg = 0;
uint16_t g_dac_init_ticks = 0;
uint8_t g_dac_init_flg = 0;
uint8_t g_reverse_flg = 0;

void dac_timer_task(void)
{
    if (g_dac_init_flg == 0)
    {
        g_dac_init_ticks++;
        DAC_0_set_output(SIN_DATA[0]);
        if (g_dac_init_ticks >= 300)
        {
            g_dac_init_flg = 1;
            MOTOR_GATE_set_level(true);
        }
    }
    else
    {
        if (g_sin_data_out_flg == 1)
        {
            if (g_sin_data_index < (sizeof(SIN_DATA) / sizeof(SIN_DATA[0])))
            {
                if (g_reverse_flg == 1)
                {
                    DAC_0_set_output(SIN_DATA_RESERVE[g_sin_data_index]);
                }
                else
                {
                    DAC_0_set_output(SIN_DATA[g_sin_data_index]);
                }
                g_sin_data_index++;
            }
            else
            {
                g_sin_data_index = 0;
                g_sin_data_out_flg = 0;
                g_reverse_flg = 0;
            }
        }
        else if (g_slide_sin_data_out_flg == 1)
        {
            if (g_slide_sin_data_index < (sizeof(SIN_DATA_SLIDE) / sizeof(SIN_DATA_SLIDE[0])))
            {
                DAC_0_set_output(SIN_DATA_SLIDE[g_slide_sin_data_index]);
                g_slide_sin_data_index++;
            }
            else
            {
                g_slide_sin_data_index = 0;
                g_slide_sin_data_out_flg = 0;
            }
        }
    }
}

void dac_run(void)
{
    g_sin_data_out_flg = 1;
    g_slide_sin_data_index = 0;
    g_slide_sin_data_out_flg = 0;
}

void dac_run_test(void)
{
    g_reverse_flg = 1;
    dac_run();
}

void dac_slide_run(void)
{
    if (g_sin_data_out_flg == 0)
    {
        g_slide_sin_data_index = 0;
        g_slide_sin_data_out_flg = 1;
    }
}

uint8_t dac_ret_status(void)
{
    return g_sin_data_out_flg;
}