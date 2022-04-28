/*
 * sine.c
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "sine.h"

static uint16_t sine_lookup[TABLE_SIZE];

/*
 *
 */
void Init_Sine()
{
   double pip2 = 3.14159/2.0;

   for(int i = 0; i < TABLE_SIZE; i++)
   {
	   double s = sin((i * pip2)/ (TABLE_SIZE-1)) * (TRIG_SCALE_FACTOR);
	   sine_lookup[i] = floor(s+0.5);
   }
}

/*
 *
 */
int fp_sin(int i)
{
	int v;

	while (i > TWO_PI)
	{
		i -= TWO_PI;
	}

	while (i < 0)
	{
		i += TWO_PI;
	}

	if (i <= PI_BY2)
	{
		v = sine_lookup[i];
	}
	else if (i <= PI)
	{
		v = sine_lookup[PI - i];
	}
	else if (i <= PI3_BY2)
	{
		v = -sine_lookup[i - PI];
	}
	else
	{
		v = -sine_lookup[TWO_PI - i];
	}

	return v;
}

/*
 *
 */
uint32_t Init_Table(uint32_t s_freq, uint32_t t_freq, int16_t *data)
{
	uint32_t num = s_freq/t_freq ;

	for (int i = 0; i < num; i++)
	{
		int indx = (TWO_PI * i)/num;
		data[i] = fp_sin(indx);
	}

	return num;
}
