/*
 * test_int_bin_conversions.c
 *
 *  Created on: Mar 26, 2022
 *      Author: lpandit
 */

/*
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "int_bin_conversions.h"

/*
 *
 */
void print_msg(char *func, int test_num, bool passed)
{
	printf("Function: %s Test:%d Result:%s\n", func, test_num, (passed) ? "PASSED" : "FAILED");
}

/*
 *
 */
void test_uint_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits, char *exp_str, int exp_return)
{
	static int test_num = 0;
	int ret_value = uint_to_binstr(str, size, (int32_t)num, nbits);

	test_num++;
	assert(ret_value == exp_return);
	assert(strcmp(str, exp_str) == 0);

	print_msg("uint_to_binstr", test_num, true);
}

/*
 *
 */
void test_int_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits, char *exp_str, int exp_return)
{
	static int test_num = 0;
	int ret_value = int_to_binstr(str, size, (int32_t)num, nbits);

	test_num++;
	assert(ret_value == exp_return);
	assert(strcmp(str, exp_str) == 0);

	print_msg("int_to_binstr", test_num, true);
}

/*
 *
 */
int main(int argc, char *argv[])
{
     const int STR_SIZE = 256;
	 char str[STR_SIZE];

     test_uint_to_binstr(str, STR_SIZE, 18, 8, "0b00010010", 10);
     test_uint_to_binstr(str, STR_SIZE, 65400, 16, "0b1111111101111000", 18);
     test_uint_to_binstr(str, STR_SIZE, 310, 0, "", -1);

     test_int_to_binstr(str, STR_SIZE, 18, 8, "0b00010010", 10);
     test_int_to_binstr(str, STR_SIZE, -1, 4, "0b1111", 6);
     test_int_to_binstr(str, STR_SIZE, -3, 8, "0b11111101", 10);

     return 0;
}
