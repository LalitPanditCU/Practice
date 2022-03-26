/*
 * func.c: List of functions for Assignment 1.
 *
 *
 *  Created on: Mar 25, 2022
 *      Author: lpandit
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

/**
 * Purpose:
 * Returns binary representation of an signed integer.
 *
 *
 * Parameters:
 *    str:
 *    size:
 *    num:
 *    nbits:
 *
 * Return:
 *    Size of the binary string not including the null characters.
 *    In case of error, it returns -1 and str is set to empty string.
 */
int int_to_binstr(char *str, size_t size, int32_t num, uint8_t nbits)
{
	int ret_val = -1;  // Return the number of characters in the string

	// Parameters check.
    if (nbits <= 0               ||            // nbits should be positive int. < 0 check is redundant.
    	nbits > size-3 			 ||            // Illegal nbits. Minimumn size is 3 for '0b' and NULL char.
		nbits > sizeof(uint32_t) * CHAR_BIT)   // nbits must be less or equal to size of uint32_t.
    {
        str[0] = '\0';
        return ret_val;
    }

    // Initialize the string and return value.
    ret_val = 2;
    strcpy(str, "0b");

    // Loop through each bit mask to extract the bit.
    for(int i=nbits-1; i >= 0; i--)
    {
    	uint32_t mask = 1 << i;
    	str[ret_val] = (num & mask) ? '1' : '0';

    	ret_val++;
    }

    // Finalize the string with null char.
    str[ret_val] = '\0';

    return ret_val;
}

/*
 *
 */
int uint_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits)
{
	return int_to_binstr(str, size, (int32_t)num, nbits);
}

/*
 *
 */
void test_uint_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits, char *exp_str, int exp_return)
{
	static int success = 0;
	int ret_value = uint_to_binstr(str, size, (int32_t)num, nbits);

	assert(ret_value == exp_return);
	assert(strcmp(str, exp_str) == 0);
	success += 1;
	printf("uint_to_binstr Success %d\n", success);
}

/*
 *
 */
void test_int_to_binstr(char *str, size_t size, uint32_t num, uint8_t nbits, char *exp_str, int exp_return)
{
	static int success = 0;
	int ret_value = int_to_binstr(str, size, (int32_t)num, nbits);

	assert(ret_value == exp_return);
	assert(strcmp(str, exp_str) == 0);
	success += 1;
	printf("int_to_binstr Test: %d passed.\n", success);
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
