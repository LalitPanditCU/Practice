/*
 * delay.c
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#include "my_printf.h"
#include "delay.h"

void delay(delay_t dl)
{
	int loops = (uint32_t) dl;

	MY_PRINTF("START TIMER %d msec\n",  (int)dl);
	for (; loops; loops--)
	{
		asm ("nop");
	}
}
