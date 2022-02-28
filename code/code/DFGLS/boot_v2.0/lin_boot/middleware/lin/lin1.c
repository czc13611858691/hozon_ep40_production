/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : lin1.c
**     Project     : lin_slave_s32k144
**     Processor   : S32K144_100
**     Component   : lin
**     Version     : Component SDK_S32K1xx_14, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K1xx_14
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-04-01, 16:25, # CodeGen: 0
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file lin1.c
** @version 01.00
*/
/*!
**  @addtogroup lin1_module lin1 module documentation
**  @{
*/

/*!
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files, with the same initialized values.
 */

/* MODULE lin1. */

#include "lin1.h"
#include "usart.h"


/*! lin1 configuration structure */
lin_user_config_t lin1_InitConfig0 = {
  .baudRate = 19200,
  .nodeFunction = (bool)SLAVE,
  .autobaudEnable = true,
  //.timerGetTimeIntervalCallback = timerGetTimeIntervalCallback0,
};


/*! Driver state structure */
lin_state_t lin1_State;


lin_func_callback_t lin1_Callback = {
  .ReadByte = (void*)USART4_Read,
  .SendByte = (void*)USART4_Send,
};

/* END lin1. */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
