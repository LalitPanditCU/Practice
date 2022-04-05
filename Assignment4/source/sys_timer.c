/*
 * sys_timer.c
 *
 *  Created on: Apr 4, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "sys_timer.h"

static volatile uint32_t total_ticks=0;
static volatile uint32_t reset_ticks=0;
static volatile int reset_flag=0;

static volatile uint32_t callback_time;
static volatile void (*callback_fptr)();

/*
 *
 */
void SysTick_Handler()
{
	total_ticks++;

	if (reset_flag)
	{
		reset_ticks = 0;
		reset_flag = 0;
	}
	else
	{
		reset_ticks++;
		if (reset_ticks == callback_time)
		{
			callback_fptr();
		}
	}
}

/*
 *
 */
void init_systick()
{

	/* 1. Program reload value.
	 * 2. Clear current value.
	 * 3. Program Control and Status register.
	 */
	  uint32_t ticks = 3000000;  // 48MHz / 16 to get 16 msec interrupt.

	  SysTick->LOAD  = (uint32_t)(ticks - 1UL);     // Subtract 1 for number of ticks between interrups, set reload register */
	  // NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
	  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
	                   SysTick_CTRL_TICKINT_Msk   |
	                   SysTick_CTRL_ENABLE_Msk;
}

/*
 *
 */
ticktime_t now()
{
	return total_ticks;
}

/*
 *
 */
void reset_timer()
{
    reset_flag = 1;
}

/*
 *
 */
ticktime_t get_timer()
{
    return reset_ticks;
}

/*
 *
 */
void set_callback(uint32_t c_time, void (*c_fn)())
{
	callback_time = c_time;
	callback_fptr = c_fn;
}
