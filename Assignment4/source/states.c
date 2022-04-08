/*
 * states.c
 *
 *  Created on: Apr 4, 2022
 *      Author: lpandit
 */

#include "fsl_debug_console.h"
#include "sys_timer.h"
#include "leds.h"
#include "touch.h"
#include "states.h"

typedef enum {s_STOP, s_STOP_GO, s_GO, s_GO_WARNING, s_WARNING, s_WARNING_STOP,
	          s_TO_CROSSWALK, s_CROSSWALK, s_CROSSWALK_GO} states_t;
#define STOP_COLOR      0x611E3C
#define GO_COLOR        0x229622
#define WARNING_COLOR   0xFFB200
#define CROSSWALK_COLOR 0x001030
#define CROSSWALK_RED    0x00
#define CROSSWALK_GREEN  0x10
#define CROSSWALK_BLUE   0x30

#define CROSSWALK_ON_TICKS 6
#define CROSSWALK_OFF_TICKS 4

#ifdef NDEBUG
#define STOP_TIME 320
#define STOP_GO_TIME 16
#define GO_TIME 320
#define GO_WARNING_TIME 16
#define WARNING_TIME 80
#define WARNING_STOP_TIME 16
#define TO_CROSSWALK_TIME 16
#define CROSSWALK_TIME 160
#define CROSSWALK_GO_TIME 16
#else
#define STOP_TIME 80
#define STOP_GO_TIME 16
#define GO_TIME 80
#define GO_WARNING_TIME 16
#define WARNING_TIME 48
#define WARNING_STOP_TIME 16
#define TO_CROSSWALK_TIME 16
#define CROSSWALK_TIME 160
#define CROSSWALK_GO_TIME 16
#endif

uint32_t stop_go_color[] = {0x611E3C, 	0x5D263A, 0x592D39, 0x553537, 0x513C36,
										0x4D4434, 0x494B32, 0x455331, 0x425A2F,
										0x3E622D, 0x3A692C, 0x36712A, 0x327829,
										0x2E8027, 0x2A8725, 0x268F24, 0x229622};

uint32_t go_warning_color[] = {0x229622, 0x309820, 0x3E9A1E, 0x4B9B1C, 0x599D1A,
										 0x679F17, 0x75A115, 0x83A213, 0x91A411,
										 0x9EA60F, 0xACA80D, 0xBAA90B, 0xC8AB09,
										 0xD6AD06, 0xE3AF04, 0xF1B002, 0xFFB200};

uint32_t warning_stop_color[] = {0xFFB200, 0xF5A904, 0xEBA008, 0xE1960B, 0xD88D0F,
		   	   	   	   	   	   	   	   	   0xCE8413, 0xC47B17, 0xBA711A, 0xB0681E,
										   0xA65F22, 0x9C5626, 0x924C29, 0x89432D,
										   0x7F3A31, 0x753135, 0x6B2738, 0x611E3C};

uint32_t crosswalk_go_color[] = {0x001030, 0x02182F, 0x04212E, 0x06292D, 0x09322D,
										   0x0B3A2C, 0x0D422B, 0x0F4B2A, 0x115329,
										   0x135B28, 0x156427, 0x176C26, 0x1A7526,
										   0x1C7D25, 0x1E8524, 0x208E23, 0x229622};

uint32_t to_crosswalk_color[] = {0x00, 0x00, 0x00, 0x00, 0x00,
		                               0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00};

typedef struct {
	uint32_t time;
	states_t next_state;
	uint32_t *color_ptr;
} state_info_t;

state_info_t sinfo[]={{STOP_TIME, s_STOP_GO, NULL},
		              {STOP_GO_TIME, s_GO, stop_go_color},
					  {GO_TIME, s_GO_WARNING, NULL},
					  {GO_WARNING_TIME, s_WARNING, go_warning_color},
					  {WARNING_TIME, s_WARNING_STOP, NULL},
					  {WARNING_STOP_TIME, s_STOP, warning_stop_color},
					  {TO_CROSSWALK_TIME, s_CROSSWALK, to_crosswalk_color},
					  {CROSSWALK_TIME, s_CROSSWALK_GO, NULL},
					  {CROSSWALK_GO_TIME, s_GO, crosswalk_go_color}};


static states_t c_state; // Current state
static uint32_t c_color; // Current color
static volatile int state_change;     // Flag to indicate state change
static volatile int color_ticks = 0;  // Ticks for color changes during transitions


// Flags for crosswalk state
static volatile int cross_walk = 0;   //Enter crosswalk flag.
static volatile int crosswalk_on = 0; //For blinking crosswalks
static volatile int crosswalk_color;  //Crosswalk start color
static volatile int crosswalk_ticks;  //For blinking cross walk

/*
 *
 */
void state_timer_callback(uint32_t ticks)
{
	uint32_t max_time = sinfo[(int)c_state].time;
	state_change = (ticks == max_time);
	color_ticks = ticks;
}

/*
 *
 */
void state_touch_callback()
{
    cross_walk = 1;
}

/*
 *
 */
static int _mult(int n, uint32_t m)
{
	int sum = 0;
    for (int i = 0; i < m; i++)
    {
    	sum += n;
    }

    return sum;
}

/*
 *
 */
static uint32_t cx_color(uint8_t v1, uint8_t v2, uint8_t ticks)
{
	int v = (v2 - v1) >> 4;
	return v1 + _mult(v, ticks);
}

/*
 *
 */
static void _fill_crosswalk_colors()
{
	uint8_t c_red = c_color >> 16;
	uint8_t c_green = c_color >> 8 & 0xFF;
	uint8_t c_blue = c_color & 0xFF;

	uint32_t *color_ptr = sinfo[(int)s_TO_CROSSWALK].color_ptr;

	for (int i = 0; i < 18; i++)
	{
		uint8_t n_red = cx_color(c_red, CROSSWALK_RED, i);
		uint8_t n_green = cx_color(c_green, CROSSWALK_GREEN, i);
		uint8_t n_blue = cx_color(c_blue, CROSSWALK_BLUE, i);

		uint32_t n_color = (n_red << 16) + (n_green << 8) + n_blue;
		color_ptr[i] = n_color;
	}

}

/*
 *
 */
void state_loop()
{
    uint32_t *color_ptr;

     while(1)
     {

     	TSI0->DATA |= TSI_DATA_SWTS_MASK;
     	asm volatile ("WFI");

    	 // Check entry in the cross walk.
    	 if (cross_walk)
    	 {
    		 if (c_state != s_TO_CROSSWALK && c_state != s_CROSSWALK)
    		 {
    			 c_state = s_TO_CROSSWALK;
    			 state_change = 0;
    			 color_ticks = 0;
    			 crosswalk_ticks = 0;

                 _fill_crosswalk_colors();

    			 //Set led colors
    			 reset_timer();
#ifdef DEBUG
    			 PRINTF("Now %d, Ticks %d, State %d\n\r", now(), get_timer(), c_state);
#endif
    		 }

    		 cross_walk = 0;
    	 }

    	 // Check state change and regular trasnition of states.
    	 if (state_change)
    	 {
    		 state_change = 0;
    		 c_state = sinfo[(int)c_state].next_state;
    		 color_ticks = 0;

    		 reset_timer();
    		 /*
#ifdef DEBUG
    		 PRINTF("Now %d, Ticks %d, State %d\n\r", now(), get_timer(), c_state);
#endif		*/
    	 }

    	 // If state is cross walk, lights have to be blinked.
    	 if (c_state == s_CROSSWALK)
    	 {
    		 if (crosswalk_on && crosswalk_ticks == CROSSWALK_ON_TICKS)
    		 {
    			 crosswalk_on = 0;
    			 set_led_colors(0);
    			 crosswalk_ticks = 0;
    		 }
    		 else if (!crosswalk_on && crosswalk_ticks == CROSSWALK_OFF_TICKS)
    		 {
    			 crosswalk_on = 1;
    			 set_led_colors(CROSSWALK_COLOR);
    			 crosswalk_ticks = 0;
    		 }
    	 }
    	 else
    	 {
        	 // Update the color ticks
    		 color_ptr = sinfo[(int)c_state].color_ptr;
    		 if (color_ptr)
    		 {
    			 c_color = color_ptr[color_ticks];
    			 set_led_colors(c_color);
    		 }
    	 }

     }

}

/*
 *
 */
void init_states()
{
	c_state = s_STOP;
	state_change = 0;

    timer_callback_fnc(state_timer_callback);
    set_led_colors(STOP_COLOR);
    reset_timer();

    touch_callback_fnc(state_touch_callback);
}
