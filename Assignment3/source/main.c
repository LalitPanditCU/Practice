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
/* Insert other include files here. */
#include "delay.h"
#include "leds.h"
#include "touch.h"

/* Other declarations */
typedef enum colors
{
	c_RED,
	c_GREEN,
	c_BLUE,
	c_WHITE
} colors_t;

#define CHECK_PRESS(val, c_val) if (((val) != (c_val)) && ((val) != t_NONE)) break;

/*
 *
 */
void init_test_sequence()
{
	red_led_on();
	delay(d_500);

	red_led_off();
	delay(d_100);

	green_led_on();
	delay(d_500);

	green_led_off();
	delay(d_100);

	blue_led_on();
	delay(d_500);

	blue_led_off();
	delay(d_100);

	all_leds_on();
	delay(d_100);
	all_leds_off();
	delay(d_100);

	all_leds_on();
	delay(d_100);
	all_leds_off();
	delay(d_100);
}

/*
 *
 */
void run_sequence(colors_t color)
{
	delay_t on_delay[4] = {d_500, d_1000, d_2000, d_3000};
	touch_t press=t_NONE;

    for (int i = 0; i < 4; i++)
    {
    	switch(color)
    	{
    	case c_RED:
    		red_led_on();
    		press=delay_with_touch_check(on_delay[i], t_LEFT);
    		red_led_off();
    		CHECK_PRESS(press, t_LEFT);
    		press=delay_with_touch_check(d_500, t_LEFT);
    		break;

    	case c_GREEN:
    		green_led_on();
    		press=delay_with_touch_check(on_delay[i], t_MID);
    		green_led_off();
    		CHECK_PRESS(press, t_MID);
    		press=delay_with_touch_check(d_500, t_MID);
    		break;

    	case c_BLUE:
    		blue_led_on();
    		press=delay_with_touch_check(on_delay[i], t_RIGHT);
    		blue_led_off();
    		CHECK_PRESS(press, t_RIGHT);
    		press=delay_with_touch_check(d_500, t_RIGHT);
    		break;

    	case c_WHITE:
    		all_leds_on();
    		press=delay_with_touch_check(d_100, t_NONE);
    		all_leds_off();
    		if (press != t_NONE) break;
    		press=delay_with_touch_check(d_100, t_NONE);
    		break;
    	}

    	switch (press){
    	case t_LEFT:
    		color = c_RED;
    		break;
    	case t_MID:
    		color = c_GREEN;
    		break;
    	case t_RIGHT:
    		color = c_BLUE;
    		break;
    	case t_NONE:
    		break;
    	}

    	if (i == 3)
    	{
    		i = 0;
    	}
    }

}

/*
 * @brief   Application entry point.
 */
int main(void) {

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    init_leds();
    init_touch();
    init_test_sequence();

    /* Enter an infinite loop. */
    run_sequence(c_WHITE);

    return 0;
}
