/*
 * int_bin_conversions.h
 *
 *  Created on: Mar 26, 2022
 *      Author: lpandit
 */

#include <stdint.h>

#ifndef INT_BIN_CONVERSIONS_H_
#define INT_BIN_CONVERSIONS_H_

typedef enum {
	CLEAR,
	SET,
	TOGGLE
} operation_t;

int int_to_binstr(char *str, size_t size, int32_t num, uint8_t nbits);
int uint_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits);
uint32_t twiggle_bit(uint32_t input, int bit, operation_t operation);
uint32_t grab_three_bits(uint32_t input, int start_bit);
char *hexdump(char *str, size_t size, const void *loc, size_t nbytes);
int uint_to_hexstr(char *str, size_t size, uint32_t num, uint8_t nbits);

#endif /* INT_BIN_CONVERSIONS_H_ */
