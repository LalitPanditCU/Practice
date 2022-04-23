/*
 * hexdump.h
 *
 *  Created on: Apr 22, 2022
 *      Author: lpandit
 */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

#include <stdint.h>
#include <stddef.h>

char *hexdump(char *str, size_t size, const void *loc, size_t nbytes);
uint32_t aitoi(const char *s);
uint32_t ahextoi(const char *s);

#endif /* HEXDUMP_H_ */
