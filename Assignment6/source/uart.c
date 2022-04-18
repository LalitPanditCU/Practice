
/*
 * uart.c
 *
 *  Created on: Apr 17, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

#include <stdint.h>
#include "uart.h"

#define UART0_C4  0x4006A00A
#define BAUD_RATE  38400

/*
       BR = CLOCK_RATE/(SBR * (OVERSAMPLING_RATE + 1)
       BR = 24000000/(25 * 25) =
 */
#define CLOCK_RATE 24000000
#define OVERSAMPLINE_RATE 25
#define SBR        25

/*
 *
 */
void UART0_IRQHandler(void)
{
}

/*
 *
 */
void uart0_init()
{

	uint16_t sbr;

	//Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Disable transmitter and receiver
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Set the clocks
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	//Set the pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	//Select the baud rate
    *(uint32_t *)UART0_C4 |= UART0_C4_OSR(24);
    sbr = 25;




}
