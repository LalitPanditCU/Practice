
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
#define OVERSAMPLING_RATE 25
#define SBR_VAL        25

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

	//Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Disable transmitter and receiver
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Set the clocks
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	//Set the pins to UART0 Rx and Tx
	PORTA->PCR[RX_PIN] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(ALT2); // Rx
	PORTA->PCR[TX_PIN] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(ALT2); // Tx

	//Select the baud rate
    sbr = SBR_VAL;

    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(sbr>>8);
    UART0->BDL &= UART0_BDL_SBR(sbr);
    UART0->C4 |= UART0_C4_OSR(OVERSAMPLING_RATE-1);


    //Disable interrupts for RX active edge and LIN break detect, select one stop bit
    UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

    //Don't enable loopback mode, use 8 data bit mode, no parity
    UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
    //Don't invert transmit data, do enable interrupts for errors
    UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(1) | UART0_C3_NEIE(1) | UART0_C3_FEIE(1) | UART0_C3_PEIE(1);

    //Clear error flags
    UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

    //LSB first, don't invert received data
    UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

    //Enable UART transmitter and receiver
    UART0->C2  |= UART0_C2_TE(1) | UART0_C2_RE(1);

}

/*
 *
 */
void UART0_Transmit_Poll(uint8_t data)
{
	while (!(UART0->S1 & UART0_S1_TDRE_MASK))
		;
	UART0->D = data;
}

/*
 *
 */
uint8_t UART0_Receive_Poll(void)
{
	while (!(UART0->S1 & UART0_S1_RDRF_MASK))
		;

	return UART0->D;
}
