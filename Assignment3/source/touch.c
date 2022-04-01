/*
 * touch.c
 *
 *  Created on: Apr 1, 2022
 *      Author: lpandit
 */


#include "MKL25Z4.h"

//variation of the capacitance from 90 to 700
#define TOUCH_OFFSET 550  // offset value to be subtracted
#define TOUCH_DATA (TSI0->DATA & 0xFFFF)//macro for extracting the count from data register

volatile uint32_t touch_val = 0 ;

void init_touch()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; // enabling the clock

	TSI0->GENCS = TSI_GENCS_MODE(0u) | //operating in non-noise mode
								TSI_GENCS_REFCHRG(0u) | //reference oscillator charge and discharge value 500nA
								TSI_GENCS_DVOLT(0u) | //oscillator voltage rails set to default
								TSI_GENCS_EXTCHRG(0u) | //electrode oscillator charge and discharge value 500nA
								TSI_GENCS_PS(0u) |  // frequency clcok divided by one
								TSI_GENCS_NSCN(31u) | //scanning the electrode 32 times
								TSI_GENCS_TSIEN_MASK | //enabling the TSI module
								TSI_GENCS_EOSF_MASK; // writing one to clear the end of scan flag
}

int touch_scan_lh(void)
{
	unsigned int scan = 0;
	TSI0->DATA = 	TSI_DATA_TSICH(10u);
	TSI0->DATA |= TSI_DATA_SWTS_MASK; //software trigger to start the scan
	while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK )) // waiting for the scan to complete 32 times
	;
	scan = TOUCH_DATA;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; //writing one to clear the end of scan flag
	return scan- TOUCH_OFFSET;
}
