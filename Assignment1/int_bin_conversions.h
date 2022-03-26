/*
 * int_bin_conversions.h
 *
 *  Created on: Mar 26, 2022
 *      Author: lpandit
 */

#include <stdint.h>

#ifndef INT_BIN_CONVERSIONS_H_
#define INT_BIN_CONVERSIONS_H_

int int_to_binstr(char *str, size_t size, int32_t num, uint8_t nbits);
int uint_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits);

#endif /* INT_BIN_CONVERSIONS_H_ */
