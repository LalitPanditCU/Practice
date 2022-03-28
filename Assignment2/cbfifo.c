/*
 *
 */

#include <stdint.h>
#include <stddef.h>

#define CBFIFO_SIZE 128

static uint8_t cbfifo[CBFIFO_SIZE];
static uint32_t capacity=CBFIFO_SIZE;
static uint32_t read_ptr=0;
static uint32_t write_ptr=0;

/*
 *
 */
size_t cbfifo_enqueue(void *buf, size_t nbyte)
{


	return -1;
}

/*
 *
 */
size_t cbfifo_dequeue(void *buf, size_t nbyte)
{
	return -1;
}

/*
 *
 */
size_t cbfifo_length()
{
	return
}

/*
 *
 */
size_t cbfifo_capacity()
{
	return CBFIFO_SIZE;
}


