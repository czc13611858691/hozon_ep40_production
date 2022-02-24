/**
 * \file
 *
 * \brief Driver ISR.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/*
 * Code generated by START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <driver_init.h>
#include <compiler.h>
#include "soft_timer.h"
#include "lin_driver.h"
#include "lin.h"
#include "target.h"
#include "touch.h"

/* lin RX接收到一个完整数据产生的中断 */
ISR(USART4_RXC_vect)

{
   uint8_t index;
   index = (USART4.STATUS & USART_ISFIF_bm) >> USART_ISFIF_bp;
   if (index == 1)
   {
      LIN_DRV_IRQHandler(0, -1);
      USART4.STATUS |= USART_ISFIF_bm;
      return;
   }

   index = (USART4.RXDATAH & USART_DATA8_bm) >> USART_DATA8_bp;
   if (index == 0) /* is pid */
   {
      LIN_DRV_IRQHandler(0, 1);
      USART4.STATUS |= USART_ISFIF_bm;
      return;
   }

   index = (USART4.RXDATAH & USART_FERR_bm) >> USART_FERR_bp;
   if (index == 1)
   {
      // g_lin_protocol_state_array[0].error_in_response = 1;
      LIN_DRV_IRQHandler(0, -2);
   }

   LIN_DRV_IRQHandler(0, 0);
   USART4.STATUS |= USART_ISFIF_bm;
}


ISR(RTC_CNT_vect)
{

	/* Insert your RTC Compare interrupt handling code */
	touch_timer_handler();

	/* Compare interrupt flag has to be cleared manually */
	RTC.INTFLAGS = RTC_CMP_bm;
}

ISR(TCB0_INT_vect)
{
   lin_lld_timeout_service(0);
   g_soft_timer_ticks++;
   TCB0.INTFLAGS = TCB_CAPT_bm;
}

ISR(TCB1_INT_vect)
{

	TCB1.INTFLAGS = TCB_CAPT_bm;
}
