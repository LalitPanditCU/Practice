/*
 * delay.c
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#include "delay.h"

void delay(volatile uint32_t msecs)
{
	int loops = msecs >> 4;

	for (; loops; loops--)
	{
		asm ("nop");
	}
}
