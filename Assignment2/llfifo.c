/*
 * llfifo.c
 *
 *  Created on: Mar 29, 2022
 *      Author: lpandit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "llfifo.h"

typedef struct node_s{
	void *element;
	struct node_s *nxt;
} node_t;

struct llfifo_s{
	node_t *nodes;
	node_t *last;
	uint32_t length;
	uint32_t capacity;
};

#ifdef DEBUG
/*
 *
 */
void _dump_llfifo(llfifo_t *fifo)
{
	printf("LLFIFO Capacity:%d LLFIFO Length: %d\n", fifo->capacity, fifo->length);

	void *last = (fifo->last) ? fifo->last->element : NULL;
	printf("LIFO Last:%c\n", (last) ? *(char *)last : '@' );


	node_t *nodes = fifo->nodes;
	int i = 0;
	while (nodes)
	{
    	printf("%d:%02X ", i, *(char *)nodes->element);
    	if (i%16 == 0)
    	{
    		printf("\n");
    	}

    	nodes = nodes->nxt;
	}

	printf("\n");

}
#endif

/*
 *
 */
static node_t *create_node_list(int capacity)
{
	node_t *fhd=NULL,
		   *prev=NULL;

	for (int i=0; i < capacity; i++)
	{
		node_t *hd = (node_t *)malloc(sizeof(node_t));

		if (hd == NULL)
		{
			//FREE THE LIST
			return NULL;
		}

		hd->element = NULL;
		hd->nxt = NULL;

		if (i == 0)
		{
			fhd = hd;
		}
		else
		{
			prev->nxt = hd;
		}

		prev = hd;
	}

	return fhd;
}

/*
 *
 */
static void destroy_node_list(node_t *nodes)
{
	while (nodes)
	{
		node_t *prev = nodes;
		nodes = nodes->nxt;

		free(prev);
	}
}

/*
 *
 */
llfifo_t *llfifo_create(int capacity)
{
	llfifo_t *ll;

	ll = (llfifo_t *)malloc(sizeof(struct llfifo_s));
	if (ll == NULL)
	{
		return NULL;
	}

	ll->nodes = create_node_list(capacity);
	if (ll->nodes == NULL)
	{
		return NULL;
	}

    ll->last = NULL; //Last occupied
	ll->length = 0;
	ll->capacity = capacity;

	return ll;
}

/*
 *
 */
int llfifo_enqueue(llfifo_t *fifo, void *element)
{
	//Parameter check
	if (fifo == NULL || element == NULL)
	{
		return -1;
	}

	//Check if the length is less than capacity
	if (fifo->length == fifo->capacity)
	{
		fifo->last->nxt = create_node_list(fifo->capacity); //Add more capacity
		if (fifo->last->nxt == NULL)
		{
			return -1; //Error in memory allocation
		}
		fifo->capacity += fifo->capacity;
	}

	fifo->last = fifo->last->nxt;
	fifo->last->element = element;

	fifo->length += 1;

	return fifo->length;
}

/*
 *
 */
void *llfifo_dequeue(llfifo_t *fifo)
{
	if (fifo == NULL)
	{
		return NULL;
	}

	if (fifo->length > 0)
	{
		node_t *node = fifo->nodes;
		void *element = node->element;

		fifo->nodes = fifo->nodes->nxt;
		if (fifo->last == node)
		{
			fifo->last = NULL;
		}

		free (node);
		fifo->length -= 1;

		return element;
	}

	return NULL;
}

/*
 *
 */
int llfifo_length(llfifo_t *fifo)
{
	return fifo->length;
}

/*
 *
 */
int llfifo_capacity(llfifo_t *fifo)
{
	return fifo->capacity;
}

/*
 *
 */
void llfifo_destroy(llfifo_t *fifo)
{
	destroy_node_list(fifo->nodes);
	free(fifo);
}
