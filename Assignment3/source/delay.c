/*
 * delay.c
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#include "my_printf.h"
#include "touch.h"
#include "delay.h"

void delay(delay_t dl)
{
	int loops = (uint32_t) dl;

	MY_PRINTF("START TIMER %d counter\n\r",  (int)dl);
	for (; loops; loops--)
	{
		asm ("nop");
	}
}

touch_t delay_with_touch_check(delay_t dl, touch_t curr_val)
{
	int loops = (uint32_t) dl;

	MY_PRINTF("START TIMER %d counter\n\r",  (int)dl);
	int poll_time = 0;
	for (; loops; loops--)
	{
		poll_time++;
		if (poll_time == (int)d_100)
		{
			touch_t tval = touch_scan_lh();
			if (tval != curr_val && tval != t_NONE)
			{
				return tval;
			}
			else
			{
				poll_time = 0;
			}
		}
	}

	return curr_val;
}
