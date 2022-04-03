/*
 * delay.h
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stdint.h"

typedef enum d_delay{
	d_100 = 4800U << 7,
	d_500 = d_100 << 2,
	d_1000 = d_500 << 1,
	d_2000 = d_500 << 2,
	d_3000 = d_1000 + d_2000
} delay_t;

void delay(delay_t delay);


#endif /* DELAY_H_ */
