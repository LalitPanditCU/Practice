/*
 * leds.c
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

#include "my_printf.h"
#include "leds.h"


#define RED_PIN     (18)
#define GREEN_PIN   (19)
#define BLUE_PIN    (1)

#define MASK(x)  (1 << (x))

/*
 *
 */
void red_led_off()
{
	GPIOB->PSOR |= MASK(RED_PIN);

	MY_PRINTF("RED LED ON.\n\r");
}

/*
 *
 */
void red_led_on()
{
	GPIOB->PCOR |= MASK(RED_PIN);
	MY_PRINTF("RED LED OFF.\n\r");
}

/*
 *
 */
void green_led_off()
{
	GPIOB->PSOR |= MASK(GREEN_PIN);
	MY_PRINTF("GREEN LED ON.\n\r");
}

/*
 *
 */
void green_led_on()
{
	GPIOB->PCOR |= MASK(GREEN_PIN);
	MY_PRINTF("GREEN LED OFF.\n\r");
}

/*
 *
 */
void blue_led_off()
{
	GPIOD->PSOR |= MASK(BLUE_PIN);
	MY_PRINTF("BLUE LED ON.\n\r");
}

/*
 *
 */
void blue_led_on()
{
	GPIOD->PCOR |= MASK(BLUE_PIN);
	MY_PRINTF("BLUE LED OFF.\n\r");
}

/*
 *
 */
void all_leds_on()
{
	red_led_on();
	green_led_on();
	blue_led_on();
	MY_PRINTF("WHITE LED ON.\n\r");
}

/*
 *
 */
void all_leds_off()
{
	red_led_off();
	green_led_off();
	blue_led_off();
	MY_PRINTF("WHITE LED OFF.\n\r");
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
    PORTB->PCR[RED_PIN] |= PORT_PCR_MUX(1);

    PORTB->PCR[GREEN_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[GREEN_PIN] |= PORT_PCR_MUX(1);

    PORTD->PCR[BLUE_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[BLUE_PIN]  |= PORT_PCR_MUX(1);

    /* Set the port direction */
    GPIOB->PDDR |= MASK(RED_PIN);
    GPIOB->PDDR |= MASK(GREEN_PIN);
	GPIOD->PDDR |= MASK(BLUE_PIN);

	/* Turn off all leds */
	red_led_off();
	green_led_off();
	blue_led_off();

}



