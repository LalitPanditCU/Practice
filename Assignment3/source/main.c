/*
 *
 */
 
/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "delay.h"
#include "leds.h"

#define DELAY 10000000U

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n");
    init_leds();

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        delay(DELAY);
        red_led_on();
        delay(DELAY);
        red_led_off();
        delay(DELAY);
        green_led_on();
        delay(DELAY);
        green_led_off();
        delay(DELAY);
        blue_led_on();
        delay(DELAY);
        blue_led_off();
        delay(DELAY);
        all_leds_on();
        delay(DELAY);
        all_leds_off();
    }
    return 0 ;
}
