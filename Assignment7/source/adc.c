/*
 * adc.c
 *
 *  Created on: Apr 28, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

#include "adc.h"

#define ADC_POS  (30)

/*
 *
 */
void Init_ADC(void)
{
	// Dean page 173
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[ADC_POS] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[ADC_POS] |= PORT_PCR_MUX(0);

	ADC0->CFG1 = ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0);

	ADC0->SC2 = ADC_SC2_REFSEL(0);

	//Use TPM1 trigger.
	SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(9);
}

/*
 *
 */
int read_sample(void)
{
	int n;

	ADC0->SC1[0] = 0x00;

	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
		;

	n = ADC0->R[0];

	return n;
}

