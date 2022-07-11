/*
 * delay.h
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stdint.h"
#include "touch.h"

typedef enum d_delay{
	d_100 = (4800U << 7) >> 3,
	d_500 = (d_100 << 2) + d_100,
	d_1000 = d_500 << 1,
	d_2000 = d_500 << 2,
	d_3000 = d_1000 + d_2000
} delay_t;

void delay(delay_t delay);
touch_t delay_with_touch_check(delay_t delay, touch_t c_val);


#endif /* DELAY_H_ */
