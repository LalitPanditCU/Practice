/*
 * switch.c
 *
 *  Created on: Jun 6, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

#include "switch.h"

#define MASK(x)   (1 << (x))
#define SWITCH_POS (7)

static int pressed = 0;
static delay_t dl = d_3000;

/*
 *
 */
void PORTD_IRQHandler()
{

   __disable_irq();

   if (PORTD->ISFR & MASK(SWITCH_POS))
   {
	   pressed = !pressed;

	   if (pressed)
	   {
		   dl = d_100;
	   }
	   else
	   {
		   dl = d_3000;
	   }
   }

   /* Clear status flags */
   PORTD->ISFR = 0xffffffff;

   __enable_irq();
}

/**
 *
 */
delay_t get_delay()
{
	return dl;
}

/*
 *
 */
void Switch_Init()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	/* Setup the pin control register mux */
    PORTD->PCR[SWITCH_POS] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[SWITCH_POS] |= PORT_PCR_MUX(1);

    PORTD->PCR[SWITCH_POS] |= PORT_PCR_PE(1);
    PORTD->PCR[SWITCH_POS] |= PORT_PCR_PS(1);
    PORTD->PCR[SWITCH_POS] |= PORT_PCR_IRQC(10);

    GPIOD->PDDR &= ~MASK(SWITCH_POS);

    /* Enable interrupts */
	NVIC_SetPriority (PORTD_IRQn, 2); /* set Priority for Systick Interrupt */
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);

}
