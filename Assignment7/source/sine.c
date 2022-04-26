/*
 * sine.c
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */

#include <stdint.h>
#include <math.h>

#include "sine.h"

static uint16_t sine_lookup[6435];

/*
 *
 */
void init_sine()
{
   double pip2 = 3.14159/2.0;

   for(int i = 0; i <= 6434; i++)
   {
	   double s = sin((i * pip2)/ 6434) * (4096);
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

	if (i >= 0 && i <= PI_BY2)
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
