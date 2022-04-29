/*
 * dac.c
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */

#include <MKL25Z4.h>

#include <dac.h>

#define DAC_POS  (30)

void Init_DAC(void) {
  // Init DAC output

	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[DAC_POS] &= ~(PORT_PCR_MUX(7));	// Select analog

	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}

