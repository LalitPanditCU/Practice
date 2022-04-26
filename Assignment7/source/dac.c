/*
 * dac.c
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */

#include <MKL25Z4.h>

#include <dac.h>

#define DAC_POS  (30)

void Init_DAC (void)
{
	// Enable clock to DAC and Port E
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Select analog out for the pin
	PORTE->PCR[DAC_POS] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[DAC_POS] |= PORT_PCR_MUX(0);

	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	// Enable DAC, select VDDA as a reference voltage
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}
