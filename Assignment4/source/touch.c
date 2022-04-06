/*
 * touch.c
 *
 *  Created on: Apr 1, 2022
 *      Author: lpandit
 */


//************************************************ Pre-Processor Directives *****************************************
#include <MKL25Z4.H>
#include "touch.h"

//variation of the capacitance from 90 to 700
#define TOUCH_OFFSET 550  // offset value to be subtracted
#define TOUCH_DATA (TSI0->DATA & 0xFFFF)//macro for extracting the count from data register
#define MIN_TOUCH_VAL 100

//************************************************Global Variables*********************************************
static volatile void (*callback_fptr)();

//************************************************Function Prototypes*********************************************
void TSI0_IRQHandler (void){
	int touch_val = TOUCH_DATA - TOUCH_OFFSET;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; 	//writing one to clear the flag

	if (touch_val > MIN_TOUCH_VAL)
	{
		callback_fptr();
	}
}

void touch_callback_fnc(void (*c_fn)(uint32_t ticks))
{
	callback_fptr = c_fn;
}

void init_touch()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; // enabling the clock

	TSI0->GENCS = TSI_GENCS_MODE(0u) |
								TSI_GENCS_REFCHRG(0u) |
								TSI_GENCS_DVOLT(0u) |
								TSI_GENCS_EXTCHRG(0u) |
								TSI_GENCS_PS(0u) |
								TSI_GENCS_NSCN(31u) |
								TSI_GENCS_TSIEN_MASK |
								TSI_GENCS_STPE_MASK |  // enabling the TSI in low power modes
								TSI_GENCS_EOSF_MASK |
								TSI_GENCS_ESOR_MASK | //enabling interrupt using end of scan
								TSI_GENCS_TSIIEN_MASK; //enabling the TSI interrupt

	TSI0->DATA = 	TSI_DATA_TSICH(10u); // selecting channel 10
	//enaling interrupt in NVIC
	NVIC_SetPriority(TSI0_IRQn, 2);
	NVIC_ClearPendingIRQ(TSI0_IRQn);
	NVIC_EnableIRQ(TSI0_IRQn);
	// Allow low leakage stop mode
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK; //
	// Enable low-leakage stop mode and regular run mode
	SMC->PMCTRL = SMC_PMCTRL_STOPM(3) | SMC_PMCTRL_RUNM(0);
	SMC->STOPCTRL = SMC_STOPCTRL_PSTOPO(0) | SMC_STOPCTRL_VLLSM(3);
	// Enable LLWU
	// allow TSI0 to wake LLWU
	LLWU->ME |= LLWU_ME_WUME4_MASK;
}

