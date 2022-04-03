/*
 * touch.c
 *
 *  Created on: Apr 1, 2022
 *      Author: lpandit
 */


#include "MKL25Z4.h"

#include "my_printf.h"
#include "touch.h"

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

touch_t touch_scan_lh(void)
{
    int pressed = 0;
	unsigned int scan = 0;
	TSI0->DATA = 	TSI_DATA_TSICH(10u);
	TSI0->DATA |= TSI_DATA_SWTS_MASK; //software trigger to start the scaN

	for (int i=0; i < 10000 || (!pressed); i++) // waiting for the scan to complete 32 times
			pressed = (TSI0->GENCS & TSI_GENCS_EOSF_MASK);
		;

	scan = TOUCH_DATA;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; //writing one to clear the end of scan flag

	if (!pressed){
		return t_NONE;
	}

	int sval = scan - TOUCH_OFFSET;
	MY_PRINTF ("SLIDER VALUE %d\n\r", sval);


	if (sval < 100)
	{
		return t_NONE;
	}
	else if (sval < 300)
	{
	    return t_LEFT;
	}
	else if (sval > 700)
	{
		return t_RIGHT;
	}

	return t_MID;
}
