/*
 * tmp.c
 *
 *  Created on: Apr 27, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

#include "tpm.h"

#define CLOCK_RATE    48000000 // 48MHz clock
#define SAMPLING_RATE 48000 // 48KHz sampling rate
#define TPM0_MOD_VAL       (1000 - 1)  // CLOCK_RATE/SAMPLING_RATE - 1

#define TPM1_MOD_VAL  (500 - 1)

/*
 *
 */
void Init_TPM0(void)
{
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	TPM0->SC = 0;

	TPM0->MOD = TPM0_MOD_VAL;

	TPM0->SC = TPM_SC_CMOD(1) | TPM_SC_DMA_MASK;
}


/*
 *
 */
void Init_TPM1(void)
{
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	TPM1->SC = 1;

	TPM1->MOD = TPM1_MOD_VAL;

	TPM1->SC = TPM_SC_CMOD(1) | TPM_SC_DMA_MASK;
}

/*
 *
 */
void Init_TPM(void)
{
      Init_TPM0();
      Init_TPM1();
}
