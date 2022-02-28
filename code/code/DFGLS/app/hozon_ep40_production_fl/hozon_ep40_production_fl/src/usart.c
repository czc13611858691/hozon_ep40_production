/**
 * \file
 *
 * \brief USART init driver.
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

/**
 * \defgroup doc_driver_usart_init USART Init
 * \ingroup doc_driver_usart
 *
 * \section doc_driver_usart_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <usart.h>
#include "clock_config.h"
#include "target.h"

/**
 * \brief Initialize usart interface
 *
 * \return Initialization status.
 */
int8_t USART_0_init()
{

	LIN_USART_X.BAUD = (uint16_t)(((float)(F_CPU * 64 / (16 * (float)19200)) + 0.5));

	LIN_USART_X.CTRLA = 1 << USART_ABEIE_bp		/* Auto-baud Error Interrupt Enable: enabled */
				   | 0 << USART_DREIE_bp	/* Data Register Empty Interrupt Enable: enabled */
				   | 0 << USART_LBME_bp		/* Loop-back Mode Enable: disabled */
				   | USART_RS485_DISABLE_gc /* RS485 Mode disabled */
				   | 1 << USART_RXCIE_bp	/* Receive Complete Interrupt Enable: enabled */
				   | 0 << USART_RXSIE_bp /* Receiver Start Frame Interrupt Enable: disabled */ 
				   | 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

	LIN_USART_X.CTRLB = 0 << USART_MPCM_bp		 /* Multi-processor Communication Mode: disabled */
				   | 0 << USART_ODME_bp		 /* Open Drain Mode Enable: disabled */
				   | 1 << USART_RXEN_bp		 /* Receiver Enable: enabled */
				   | USART_RXMODE_LINAUTO_gc /* LIN constrained autobaud mode */
				   | 0 << USART_SFDEN_bp	 /* Start Frame Detection Enable: disabled */
				   | 1 << USART_TXEN_bp;	 /* Transmitter Enable: enabled */

	return 0;
}
