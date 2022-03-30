/*
 * test_llfifo.c
 *
 *  Created on: Mar 30, 2022
 *      Author: Lalit
 */

#include "llfifo.h"
#include "test_llfifo.h"

void test_llfifo()
{
    char d_array[256];
	llfifo_t *fifo;

	fifo = llfifo_create(100);
	_dump_llfifo(fifo);

	int el1 = 33;
	llfifo_enqueue(fifo, &el1);
	_dump_llfifo(fifo);
	llfifo_dequeue(fifo);
	_dump_llfifo(fifo);

	for (int i=65; i < 200; i++)
	{
		d_array[i] = i;
		llfifo_enqueue(fifo, d_array+i);
		_dump_llfifo(fifo);
	}
	_dump_llfifo(fifo);
}
