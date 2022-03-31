/*
 * delay.c
 *
 *  Created on: Mar 31, 2022
 *      Author: lpandit
 */


void delay(int msecs)
{
	int loops = msecs >> 4;

	for (; loops; loops--)
	{
		__asm volatile ("nop");
	}
}
