/*
 * touch.h
 *
 *  Created on: Apr 1, 2022
 *      Author: lpandit
 */

#ifndef TOUCH_H_
#define TOUCH_H_

typedef enum {
	t_LEFT,
	t_MID,
	t_RIGHT,
	t_NONE
} touch_t;

void init_touch();
void touch_callback_fnc(void (*c_fn)(uint32_t ticks));


#endif /* TOUCH_H_ */
