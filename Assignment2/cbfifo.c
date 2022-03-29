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

#define CBFIFO_EMPTY  ((read_ptr == write_ptr) && (!is_full))

#ifdef DEBUG
/*
 *
 */
static void _dump_cbfifo()
{
	size_t len = cbfifo_length();
	printf("CBFIFO Length: %d\n", len);

	printf("Read_ptr: %d, Write_ptr: %d, Is_full%d\n", read_ptr, write_ptr, is_full, is_full);

	uint32_t tmp_ptr = read_ptr;
	for (i = 0; i < len; i++)
	{
    	ch = *(cbfifo+tmp_ptr);
    	printf("%d:%02X ", tmp_ptr, ch);
    	if (i%16 == 0)
    	{
    		printf("\n");
    	}

    	tmp_ptr = (tmp_ptr+1) & cbfifo_mask;
	}

	printf("\n");

}
#endif

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
    	*(cbfifo+write_ptr) = *((char *)(buf+i));
    	write_ptr = (write_ptr+1) & cbfifo_mask;
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
    if (CBFIFO_EMPTY) //Is empty
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
    	*((char *)(buf+i)) = *(cbfifo+read_ptr);
    	read_ptr = (read_ptr+1) & cbfifo_mask;
    }

    if (nbyte > 0)
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
	if (is_full)
	{
		return capacity;
	}
	else
	{
		return (write_ptr - read_ptr) & cbfifo_mask;

	}
}
/*
 *
 */
size_t cbfifo_capacity()
{
	return CBFIFO_SIZE;
}


