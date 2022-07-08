/*
 * func.c: List of functions for Assignment 1.
 *
 *
 *  Created on: Mar 25, 2022
 *      Author: lpandit
 */

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

#include "int_bin_conversions.h"

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
    	str[ret_val++] = (num & mask) ? '1' : '0';
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

static char nibble_to_hex(uint8_t byte)
{
	const char hex_map[]="0123456789ABCDEF";

	return hex_map[byte & 0b1111];
}

/*
 *
 */
static char *byte_to_hex(char *str, uint8_t byte)
{

	*str++ = nibble_to_hex((byte & (0b1111<<4))>>4);
	*str++ = nibble_to_hex(byte);

	return str;
}

/*
 *
 */
int uint_to_hexstr(char *str, size_t size, uint32_t num, uint8_t nbits)
{
	int err = 0;
	char *c_str=str;

	//Parameter check.
	int nib_count = nbits >> 2;
	err = size < (nib_count+2+1); //Number of hex digits plus 0x plus new line char.

	err = err || !(nbits == 4 || nbits == 8 || nbits == 16 || nbits == 32);

	//Error return.
	if (err)
	{
		*str = '\0';
		return -1;
	}

	*c_str++ = '0';
	*c_str++ = 'x';

	for (int i = nbits - 4; i >= 0; i -= 4)
	{
		uint32_t v = (num & (0b1111 << i)) >> i;

		*c_str++ = nibble_to_hex(v);
	}

	*c_str = '\0';

	return c_str - str;
}

/*
 *
 */
uint32_t twiggle_bit(uint32_t input, int bit, operation_t operation)
{
    //Parameter check
	if (bit < 0 || bit > 31)
	{
		return 0xFFFFFFFF;
	}

	// Perform the twiggle
	switch (operation)
	{
		case SET:
			input |= (1<<bit);
			break;

		case CLEAR:
			input &= ~(1<<bit);
			break;

		case TOGGLE:
			input ^= (1<<bit);
			break;
	}

	return input;
}

/*
 *
 */
uint32_t grab_three_bits(uint32_t input, int start_bit)
{
	//Parameter check
	if (start_bit < 0 || start_bit > (31-2))
	{
		return 0xFFFFFFFF;
	}

	return (input & (0b111 << start_bit)) >> start_bit;
}

#define CHECK_SIZE(ch_count, size, str) if (++(ch_count) >= (size)) {*(str) = '\0'; return str;}

/*
 *
 */
char *hexdump(char *str, size_t size, const void *loc, size_t nbytes)
{
    //Parameter checks. Size parameter will be checked later.
	if (nbytes <= 0)
	{
		*str = '\0';
		return str;
	}

	int line_bytes = 0; //Bytes written on a line. Max 16
	int ch_count = 0; //Count of chars
	size_t total_bytes = 0; //Total bytes written
	uint8_t *b_ptr = (uint8_t *)&total_bytes;
	int size_bytes = sizeof(total_bytes);

	char *m_str = str;   //Pointer for traversing the string

	while (total_bytes <= nbytes)
	{
		// Add the offset if the line_bytes is 0
		if (line_bytes == 0)
		{
			CHECK_SIZE(ch_count, size, str);
			*m_str++ = '0';

			CHECK_SIZE(ch_count, size, str);
			*m_str++ = 'x';

			// Write one byte at a time start with most significant byte.
			while (size_bytes--)
			{
				ch_count++;
				CHECK_SIZE(ch_count,size, str); //Each byte takes 2 chars.
				m_str = byte_to_hex(m_str, *(b_ptr+size_bytes));
			}

			// Restore size_bytes for next iteration.
			size_bytes = sizeof(total_bytes);

			CHECK_SIZE(ch_count, size, str);
			*m_str++ = ' ';

			CHECK_SIZE(ch_count, size, str);
			*m_str++ = ' ';
		}

		// Convert byte to hex
		ch_count++;
		CHECK_SIZE(ch_count, size, str);
		m_str = byte_to_hex(m_str, *((uint8_t *)loc+total_bytes));

		// Add new line if required.
		line_bytes++;
		total_bytes++;
		if (line_bytes == 16)
		{
			CHECK_SIZE(ch_count, size, str);
			*m_str++ = '\n';
			line_bytes = 0;
		}
		else
		{
			CHECK_SIZE(ch_count, size, str);
			if (total_bytes <= nbytes)
				{
					*m_str++ = ' ';
				}
		}
	}

	CHECK_SIZE(ch_count, size, str);
	*m_str = '\0';

	return str;
}

