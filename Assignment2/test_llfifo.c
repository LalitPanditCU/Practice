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

	llfifo_t *fifo;

	fifo = llfifo_create(100);
	_dump_llfifo(fifo);
}
