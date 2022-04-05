/*
 * states.c
 *
 *  Created on: Apr 4, 2022
 *      Author: lpandit
 */

#include "fsl_debug_console.h"
#include "sys_timer.h"

typedef enum {s_STOP, s_STOP_GO, s_GO, s_GO_WARNING, s_WARNING, s_WARNING_STOP, s_CROSSWALK, s_CROSSWALK_GO} states_t;

typedef struct {
	uint32_t time;
	states_t next_state;
} state_info_t;

#ifdef NDEBUG
state_info_t sinfo[]={{320, s_STOP_GO},
		              {16, s_GO},
					  {320, s_GO_WARNING},
					  {16, s_WARNING},
					  {80, s_WARNING_STOP},
					  {16, s_STOP},
					  {160, s_CROSSWALK},
					  {16, s_CROSSWALK_GO}};
#else
state_info_t sinfo[]={{80, s_STOP_GO},
		              {16, s_GO},
					  {80, s_GO_WARNING},
					  {16, s_WARNING},
					  {48, s_WARNING_STOP},
					  {16, s_STOP},
					  {160, s_CROSSWALK},
					  {16, s_CROSSWALK_GO}};
#endif


static states_t c_state; // Current state
static int state_change;

/*
 *
 */
void state_timer_callback(uint32_t ticks)
{
	uint32_t max_time = sinfo[(int)c_state].time;
	state_change = (ticks == max_time);
}

/*
 *
 */
void state_loop()
{
     while(1)
     {
    	 if (state_change)
    	 {
    		 c_state = sinfo[(int)c_state].next_state;
    		 state_change = 0;
    		 reset_timer();

        	 PRINTF("Current state: %d, Time in state: %d\r\n", c_state, get_timer());
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

    set_callback_fnc(state_timer_callback);
    reset_timer();
}
