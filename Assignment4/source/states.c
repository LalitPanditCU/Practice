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
	          s_CROSSWALK, s_CROSSWALK_GO} states_t;
#define STOP_COLOR      0x611E3C
#define GO_COLOR        0x229622
#define WARNING_COLOR   0xFFB200
#define CROSSWALK_COLOR 0x001030

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

typedef struct {
	uint32_t time;
	states_t next_state;
	uint32_t color;
} state_info_t;

#ifdef NDEBUG
state_info_t sinfo[]={{320, s_STOP_GO, STOP_COLOR},
		              {16, s_GO, STOP_COLOR},
					  {320, s_GO_WARNING, GO_COLOR},
					  {16, s_WARNING, GO_COLOR},
					  {80, s_WARNING_STOP, WARNING_COLOR},
					  {16, s_STOP, WARNING_COLOR},
					  {160, s_CROSSWALK, CROSSWALK_COLOR},
					  {16, s_CROSSWALK_GO, CROSSWALK_COLOR}};
#else
state_info_t sinfo[]={{80, s_STOP_GO, STOP_COLOR},
		              {16, s_GO, STOP_COLOR},
					  {80, s_GO_WARNING, GO_COLOR},
					  {16, s_WARNING, GO_COLOR},
					  {48, s_WARNING_STOP, WARNING_COLOR},
					  {16, s_STOP, WARNING_COLOR},
					  {160, s_CROSSWALK, CROSSWALK_COLOR},
					  {16, s_CROSSWALK_GO, CROSSWALK_COLOR}};
#endif


static states_t c_state; // Current state
static int state_change;
static int color_ticks = 0;

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

}

/*
 *
 */
void state_loop()
{
    uint32_t color;

     while(1)
     {
    	 if (state_change)
    	 {
    		 state_change = 0;
    		 c_state = sinfo[(int)c_state].next_state;
    		 reset_timer();
    		 color_ticks = 0;
    	 }

    	 switch(c_state)
    	 {
    	 case s_STOP_GO:
    		 color = stop_go_color[color_ticks];
    		 set_led_colors(color);
    	 	 break;
    	 case s_GO_WARNING:
    		 color = go_warning_color[color_ticks];
    		 set_led_colors(color);
    		 break;
    	 case s_WARNING_STOP:
    		 color = warning_stop_color[color_ticks];
    		 break;
    	 case s_CROSSWALK_GO:
    		 color = crosswalk_go_color[color_ticks];
    		 set_led_colors(color);
    		 break;

    	 case

    	 default:
    		 break;
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
