/*
 *
 */

#include <stdint.h>
#include <stddef.h>

#include "cbfifo.h"

#define CBFIFO_SIZE 128
const uint32_t cbfifo_mask=(CBFIFO_SIZE-1);

static uint8_t cbfifo[CBFIFO_SIZE];
static uint32_t capacity=CBFIFO_SIZE;
static uint32_t read_ptr=0;
static uint32_t write_ptr=0;
static int is_full=0;

/*
 *
 */
static size_t cbfifo_vacancies()
{
	return capacity-cbfifo_length() ;
}

/*
 *
 */
size_t cbfifo_enqueue(void *buf, size_t nbyte)
{

    if (is_full)
    {
    	return 0;
    }

    size_t spaces = cbfifo_vacancies();

    if (spaces < nbyte)
    {
    	nbyte = spaces;
    }

    for (int i = 0; i < nbyte; i++)
    {
    	write_ptr = (write_ptr+1) & cbfifo_mask;
    	*(cbfifo+write_ptr) = *((char *)(buf+i));
    }

    if (spaces == nbyte)
    {
    	is_full = 1;
    }

	return nbyte;
}

/*
 *
 */
size_t cbfifo_dequeue(void *buf, size_t nbyte)
{
    if (!is_full)
    {
    	return 0;
    }

    size_t len = cbfifo_length();

    if (len < nbyte)
    {
    	nbyte = len;
    }

    for (int i = 0; i < nbyte; i++)
    {
    	*(cbfifo+write_ptr) = *((char *)(buf+i));
    	write_ptr = (write_ptr+1) & cbfifo_mask;
    }

    if (len == nbyte)
    {
    	is_full = 0;
    }

	return nbyte;
}


/*
 *
 */
size_t cbfifo_length()
{
	return (write_ptr - read_ptr) & cbfifo_mask;
}

/*
 *
 */
size_t cbfifo_capacity()
{
	return CBFIFO_SIZE;
}


