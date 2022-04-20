
/*
 * uart.c
 *
 *  Created on: Apr 17, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

#include <stdint.h>
#include "sysclock.h"
#include "uart.h"

/*
       BR = CLOCK_RATE/(SBR * (OVERSAMPLING_RATE + 1)
       BR = 24000000/(25 * 25) = 38400
 */

#define BAUD_RATE 38400

#define OVERSAMPLING_RATE (16)
#define SBR_VAL  (39)

#define RX_PIN 1
#define TX_PIN 2
#define ALT2   2

/*
 *
 */
void UART0_IRQHandler(void)
{
}

/*
 *
 */
void UART0_init()
{

	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock MHz clock
	SIM->SOPT2 &= ~(SIM_SOPT2_UART0SRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK);
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(0);

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] =  PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] =  PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)SBR_VAL;
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(OVERSAMPLING_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0) | UART_C1_PT(0);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	//UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
	//								UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

}

/*
 *
 */
void UART0_Transmit_Poll(uint8_t data)
{
	while (!(UART0->S1 & UART0_S1_TDRE_MASK))
		__asm volatile ("nop");

	UART0->D = data;
}

/*
 *
 */
uint8_t UART0_Receive_Poll(void)
{
	while (!(UART0->S1 & UART0_S1_RDRF_MASK))
		__asm volatile ("nop");

	return UART0->D;
}

/*
 *
 */
void UART0_Send_String_Poll(uint8_t * str) {
	// enqueue string
	while (*str != '\0') { // Send characters up to null terminator
		UART0_Transmit_Poll(*str++);
	}
}
