/*
 * leds.c
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

#include "leds.h"


#define RED_PIN     (18)
#define GREEN_PIN   (19)
#define BLUE_PIN    (1)

/*
 *
 */
void red_led_on(uint8_t level)
{
   level = level << 2;
   TPM2->CONTROLS[0].CnV = level;
}

/*
 *
 */
void green_led_on(uint8_t level)
{
   level = level << 2;
   TPM2->CONTROLS[1].CnV = level;
}

/*
 *
 */
void blue_led_on(uint8_t level)
{
	level = level << 2;
	TPM0->CONTROLS[1].CnV = level;
}

/*
 *
 */
void set_led_colors(uint32_t color)
{
	uint8_t red = color >> 16;
	uint8_t green = (color >> 8) & 0xFF;
	uint8_t blue = color & 0xFF;

	red_led_on(red);
	green_led_on(green);
	blue_led_on(blue);
}

/*
 *
 */
void init_leds()
{
	/* Enable clock for the ports. */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;  //For red(PTB 18) and green led(PTB19).
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;  //For blue led(PTD1).

	/* Setup the pin control register mux */
    PORTB->PCR[RED_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[RED_PIN] |= PORT_PCR_MUX(3); //Alt3

    PORTB->PCR[GREEN_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[GREEN_PIN] |= PORT_PCR_MUX(3); //Alt3

    PORTD->PCR[BLUE_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[BLUE_PIN]  |= PORT_PCR_MUX(4);   //Alt4

	// Enable clock gating for TPMs */
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK; //For red(TPM2 Ch0) and green(TPM2 Ch1)
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; //For blue. TPM0 Ch 1

    /* Select TPM clock. */
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;

	//Use ps of 128 and mod of 1024. Intensity level is 0-255
	TPM0->MOD = 1023;  //Period - 1
	TPM0->SC |= TPM_SC_PS(7); //Prescalar of 128
	// Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);

	TPM2->MOD = 1023;
	TPM2->SC |= TPM_SC_PS(7); //Prescalar of 128
	// Continue operation in debug mode
	TPM2->CONF |= TPM_CONF_DBGMODE(3);

	//Set the counter for up counting.
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;  //Edge aligned, Low true
	TPM0->CONTROLS[1].CnV = 0; //Initialize intensity

	//Start the TPM
	TPM0->SC |= (TPM_SC_CPWMS(0) | TPM_SC_CMOD(1)); //CPWM for up counting and CMOD for enabling counting


	TPM2->CONTROLS[0].CnV = 0;
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;  //Edge aligned, Low true

	TPM2->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;  //Edge aligned, Low true

	//Start the TPM
	TPM2->SC |= (TPM_SC_CPWMS(0) | TPM_SC_CMOD(1)); //CPWM for up counting and CMOD for enabling counting
}





