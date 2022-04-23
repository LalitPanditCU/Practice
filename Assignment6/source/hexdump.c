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

#include "hexdump.h"

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
	*str++ = nibble_to_hex(byte & 0b1111);

	return str;
}

#define CHECK_SIZE(ch_count, size, str) if (++(ch_count) >= (size)) {*(str) = '\0'; return str;}

/*
 *
 */
int _hexstart(const char *s)
{
    if (*s == '\0')
    	return 0;

    return (*s == '0' && (*(s+1) == 'x' || *(s+1) == 'x'));
}

/*
 *
 */
uint32_t aitoi(const char *s)
{
	uint32_t sum = 0;

	if (*s == '\0')
		return -1;

	if (_hexstart(s))
	{
		return ahextoi(s+2);
	}

	while (*s)
	{
		if (*s >= '0' && *s <= '9')
		{
			sum = (sum * 10) + (*s - '0');
		}
		else
		{
			return -1;
		}
	}

	return sum;
}

/*
 *
 */
uint32_t ahextoi(const char *s)
{
	uint32_t sum = 0;

	if (*s == '\0')
		return -1;

	if (_hexstart(s))
	{
	   s += 2;
	}

	while (*s)
	{
		sum *= 16;
		if (*s >= '0' && *s <= '9')
		{
			sum += (*s - '0');
		}
		else if (*s >= 'A' && *s <= 'F')
		{
			sum += (10 + (*s - 'A'));
		}
		else if (*s >= 'a' && *s <= 'f')
		{
			sum +=  (10 + (*s - 'a'));
		}
		else
		{
			return -1;
		}
	}

	return sum;
}

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

