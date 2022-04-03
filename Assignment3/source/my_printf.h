/*
 * my_printf.h
 *
 *  Created on: Apr 3, 2022
 *      Author: lpandit
 */

#ifndef MY_PRINTF_H_
#define MY_PRINTF_H_

#include "fsl_debug_console.h"

#ifdef NDEBUG
#define MY_PRINTF(...)
#else
#define MY_PRINTF(...) PRINTF(__VA_ARGS__)
#endif

#endif /* MY_PRINTF_H_ */
