/*
 * sys_timer.h
 *
 *  Created on: Apr 4, 2022
 *      Author: lpandit
 */

#ifndef SYS_TIMER_H_
#define SYS_TIMER_H_

#include "stdint.h"

typedef uint32_t ticktime_t;

void init_systick();

ticktime_t now();

void reset_timer();

ticktime_t get_timer();


#endif /* SYS_TIMER_H_ */
