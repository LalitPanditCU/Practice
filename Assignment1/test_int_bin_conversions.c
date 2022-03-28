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
void check_assertions(char *func_name, int test_num, char *str, char *exp_str, int ret_value, int exp_return)
{
	assert(ret_value == exp_return);
	assert(strcmp(str, exp_str) == 0);

	print_msg(func_name, test_num, true);
}

/*
 *
 */
void test_uint_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits, char *exp_str, int exp_return)
{
	static int test_num = 0;
	int ret_value = uint_to_binstr(str, size, (int32_t)num, nbits);

	test_num++;
	check_assertions("uint_to_binstr", test_num, str, exp_str, ret_value, exp_return);
}

/*
 *
 */
void test_int_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits, char *exp_str, int exp_return)
{
	static int test_num = 0;
	int ret_value = int_to_binstr(str, size, (int32_t)num, nbits);

	test_num++;
	check_assertions("int_to_binstr", test_num, str, exp_str, ret_value, exp_return);
}

/*
 *
 */
void test_uint_to_hexstr(char *str, size_t size, uint32_t num, uint8_t nbits, char *exp_str, int exp_return)
{
	static int test_num = 0;
	int ret_value = uint_to_hexstr(str, size, num, nbits);

	test_num++;
	check_assertions("uint_to_hexstr", test_num, str, exp_str, ret_value, exp_return);
}

/*
 *
 */
void test_twiggle_bit(uint32_t input, int bit, operation_t op, int exp_return)
{
     static int test_num = 0;
     int ret_value = twiggle_bit(input, bit, op);

     test_num++;
     assert(ret_value == exp_return);
     print_msg("twiggle_bit", test_num, true);
}

/*
 *
 */
void test_grab_three_bits(uint32_t num, int bit, uint32_t exp_return)
{
	static int test_num = 0;
	int ret_value = grab_three_bits(num, bit);

	test_num++;
	assert(ret_value == exp_return);
	print_msg("grab_three_bits", test_num, true);
}

/*
 *
 */
void test_hexdump(char *str, size_t size, const void *loc, size_t nbytes, char *exp_str)
{
    static int test_num = 0;
	char *ret_str = hexdump(str, size, loc, nbytes);

	test_num++;
	assert(ret_str == str);
	assert(strcmp(ret_str, exp_str) == 0);
	print_msg("hexdump", test_num, true);
}

/*
 *
 */
int main(int argc, char *argv[])
{
     size_t STR_SIZE = 1024;
	 char str[STR_SIZE];

     test_uint_to_binstr(str, STR_SIZE, 18, 8, "0b00010010", 10);
     test_uint_to_binstr(str, STR_SIZE, 65400, 16, "0b1111111101111000", 18);
     test_uint_to_binstr(str, STR_SIZE, 310, 0, "", -1);

     test_int_to_binstr(str, STR_SIZE, 18, 8, "0b00010010", 10);
     test_int_to_binstr(str, STR_SIZE, -1, 4, "0b1111", 6);
     test_int_to_binstr(str, STR_SIZE, -3, 8, "0b11111101", 10);

     test_uint_to_hexstr(str, STR_SIZE, 18, 8, "0x12", 4);
     test_uint_to_hexstr(str, STR_SIZE, 18, 16, "0x0012", 6);
     test_uint_to_hexstr(str, STR_SIZE, 65400, 16, "0xFF78", 6);
     test_uint_to_hexstr(str, STR_SIZE, 310, 16, "0x0136", 6);

     test_twiggle_bit(0, 0, SET, 1);
     test_twiggle_bit(0, 3, SET, 8);
     test_twiggle_bit(0x7337, 5, TOGGLE, 0x7317);

     test_grab_three_bits(0x7337, 6, 4);
     test_grab_three_bits(0x7337, 7, 6);

     char *loc =                                              \
    		    "To achieve great things, two things are needed:\n" \
    		    "a plan, and not quite enough time.";

     char *ret =                                               \
    		 "0x00000000  54 6F 20 61 63 68 69 65 76 65 20 67 72 65 61 74\n" \
    		 "0x00000010  20 74 68 69 6E 67 73 2C 20 74 77 6F 20 74 68 69\n" \
    		 "0x00000020  6E 67 73 20 61 72 65 20 6E 65 65 64 65 64 3A 0A\n" \
    		 "0x00000030  61 20 70 6C 61 6E 2C 20 61 6E 64 20 6E 6F 74 20\n" \
    		 "0x00000040  71 75 69 74 65 20 65 6E 6F 75 67 68 20 74 69 6D\n" \
    		 "0x00000050  65 2E 00";

     test_hexdump(str, STR_SIZE, (const void *)loc, strlen(loc), ret);

     return 0;
}
