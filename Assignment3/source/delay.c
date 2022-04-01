/*
 * delay.c
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */

#include "delay.h"

void delay(volatile uint32_t msecs)
{
	int loops = msecs ;

	for (; loops; loops--)
	{
		asm ("nop");
	}
}
