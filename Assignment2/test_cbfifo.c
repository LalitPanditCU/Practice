/*
 * test_cbfifo.c
 *
 *  Created on: Mar 29, 2022
 *      Author: lpandit
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "cbfifo.h"

#include "test_cbfifo.h"

/*
 *
 */
static void _check_queue(int exp_len, size_t nbyte, size_t exp_nbyte, void *exp_byte)
{
	char buf[1024];
	size_t act_nbyte;

	assert(cbfifo_length() == exp_len);
	act_nbyte = cbfifo_dequeue(buf, nbyte);
	assert(act_nbyte == exp_nbyte);
	assert(memcmp(buf, exp_byte, act_nbyte) == 0);

}

/*
 *
 */
void test_cbfifo()

{
	uint8_t buf[1024];

	//Check empty.
	_check_queue(0, 5, 0, (void *)buf);

	//Queue one char.
	cbfifo_enqueue("a", 1);
	_check_queue(1, 0, 0, "");

	//Queue two chars.
	cbfifo_enqueue("ab", 2);
	_check_queue(3, 0, 0, "");
	_check_queue(3, 2, 2, "aa");
	_check_queue(1, 2, 1, "b");

	//Fill the queue
	for (int i=0;i < 1024; i++)
	{
		buf[i] = i % 256;
	}
	cbfifo_enqueue(buf, 1024);

	//Check the queue
	_check_queue(128, 56, 56, buf);

	//Check the queue
	_check_queue(72, 1000, 72, buf+56);

	//Test passed
	printf("All tests passed\n");

}
