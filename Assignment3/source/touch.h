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
	t_RIGHT
} touch_t;

void init_touch();
touch_t touch_scan_lh();


#endif /* TOUCH_H_ */
