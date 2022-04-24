/*
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "cbfifo.h"

#define CBFIFO_SIZE 4096
const uint32_t cbfifo_mask=(CBFIFO_SIZE-1);

typedef struct {
	uint8_t cbfifo[CBFIFO_SIZE];
	uint32_t capacity;
	uint32_t read_ptr;
	uint32_t write_ptr;
	int is_full;
} cbfifo_t, *cbfifo_ptr_t;

cbfifo_t txcbfifo;
cbfifo_t rxcbfifo;

cbfifo_t *txcbfifo_ptr = &txcbfifo;
cbfifo_t *rxcbfifo_ptr = &rxcbfifo;

#define CBFIFO_EMPTY(cb)  ((cb->read_ptr == cb->write_ptr) && (!cb->is_full))
#define SEL_CBFIFO(x) (((x) == c_RX) ? rxcbfifo_ptr : txcbfifo_ptr)

/*
 *
 */
void cbfifo_init()
{
	txcbfifo_ptr->capacity = CBFIFO_SIZE;
	txcbfifo_ptr->read_ptr = 0;
	txcbfifo_ptr->write_ptr = 0;
	txcbfifo_ptr->is_full = 0;

	rxcbfifo_ptr->capacity = CBFIFO_SIZE;
	rxcbfifo_ptr->read_ptr = 0;
	rxcbfifo_ptr->write_ptr = 0;
	rxcbfifo_ptr->is_full = 0;
}

#ifdef DEBUG
/*
 *
 */
void _dump_cbfifo(cbfifotype_t xx)
{
	cbfifo_ptr_t cb = SEL_CBFIFO(xx);

	size_t len = cbfifo_length(xx);
	printf("CBFIFO Length: %d\n", len);

	printf("Read_ptr: %d, Write_ptr: %d, Is_full%d\n", cb->read_ptr, cb->write_ptr, cb->is_full);

	uint32_t tmp_ptr = cb->read_ptr;
	for (int i = 0; i < len; i++)
	{
    	char ch = *(cb->cbfifo+tmp_ptr);
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
static size_t cbfifo_vacancies(cbfifotype_t xx)
{
	cbfifo_ptr_t cb = SEL_CBFIFO(xx);

	return cb->capacity-cbfifo_length(xx) ;
}

/*
 *
 */
size_t cbfifo_enqueue(cbfifotype_t xx, void *buf, size_t nbyte)
{
	cbfifo_ptr_t cb = SEL_CBFIFO(xx);

    if (cb->is_full)
    {
    	return 0;
    }

    size_t spaces = cbfifo_vacancies(xx);

    if (spaces < nbyte)
    {
    	nbyte = spaces;
    }

    for (int i = 0; i < nbyte; i++)
    {
    	*(cb->cbfifo+cb->write_ptr) = *((char *)(buf+i));
    	cb->write_ptr = (cb->write_ptr+1) & cbfifo_mask;
    }

    if (spaces == nbyte)
    {
    	cb->is_full = 1;
    }

	return nbyte;
}

/*
 *
 */
size_t cbfifo_dequeue(cbfifotype_t xx, void *buf, size_t nbyte)
{
	cbfifo_ptr_t cb = SEL_CBFIFO(xx);

    if (CBFIFO_EMPTY(cb)) //Is empty
    {
    	return 0;
    }

    size_t len = cbfifo_length(xx);

    if (len < nbyte)
    {
    	nbyte = len;
    }

    for (int i = 0; i < nbyte; i++)
    {
    	*((char *)(buf+i)) = *(cb->cbfifo+cb->read_ptr);
    	cb->read_ptr = (cb->read_ptr+1) & cbfifo_mask;
    }

    if (nbyte > 0)
    {
    	cb->is_full = 0;
    }

	return nbyte;
}


/*
 *
 */
size_t cbfifo_length(cbfifotype_t xx)
{
	cbfifo_ptr_t cb = SEL_CBFIFO(xx);

	if (cb->is_full)
	{
		return cb->capacity;
	}
	else
	{
		return (cb->write_ptr - cb->read_ptr) & cbfifo_mask;

	}
}
/*
 *
 */
size_t cbfifo_capacity(cbfifotype_t xx)
{
	return CBFIFO_SIZE;
}


