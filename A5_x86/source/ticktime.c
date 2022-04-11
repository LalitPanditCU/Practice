/**
 * @file    ticktime.c
 * @brief   Code to map SysTick timer into msec counter
 * @author  Howdy Pierce, howdy.pierce@colorado.edu
 */


#include <time.h>
#include "ticktime.h"

static volatile ticktime_t g_now = 0;
static ticktime_t g_timer = 0;

void init_ticktime(void)
{
	return;
}


ticktime_t now(void)
{
  return (clock() * 1000)/CLOCKS_PER_SEC;
}


void reset_timer(void)
{
  g_timer = clock();
}


ticktime_t get_timer(void)
{
  return ((clock() - g_timer) * 1000)/CLOCKS_PER_SEC;
}

