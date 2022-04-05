/*
 * states.c
 *
 *  Created on: Apr 4, 2022
 *      Author: lpandit
 */

typedef enum {s_STOP, s_STOP_GO, s_GO, s_GO_WARNING, s_WARNING, s_WARNING_STOP, s_CROSSWALK} states_t;

states_t c_state=STOP; //Current state

void state_loop()
{


}
