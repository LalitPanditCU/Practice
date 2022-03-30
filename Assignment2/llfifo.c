/*
 * llfifo.c
 *
 *  Created on: Mar 29, 2022
 *      Author: lpandit
 */
#include <stdio.h>
#include <stdlib.h>
#include "llfifo.h"

typedef struct node_s{
	void *element;
	struct node_s *nxt;
} node_t;

struct llfifo_s{
	node_t *first;
	node_t *last;
	uint32_t length;
	uint32_t capacity;
};

static node_t *create_node_list(int capacity)
{
	node_t *fhd=NULL, *prev=NULL;

	for (int i=0; i < capacity; i++)
	{
		node_t *hd = (node_t *)malloc(sizeof(node_t));
		if (hd == NULL)
		{
			//FREE THE LIST
			return NULL;
		}

		if (i == 0)
		{
			fhd = hd;
			prev = hd;
		}
		else
		{
			hd->element = NULL;
			hd->nxt = NULL;
			prev->nxt = hd;
		}
	}

	return fhd;

}

llfifo_t *llfifo_create(int capacity)
{
	node_t *nd;
	llfifo_t *ll;

	ll = (llfifo_t *)malloc(sizeof(struct llfifo_s));
	if (ll == NULL)
	{
		return NULL;
	}

    ll->first = create_node_list(capacity);
    ll->last = ll->first;
	ll->length = 0;
	ll->capacity = capacity;

	return ll;
}

int llfifo_enqueue(llfifo_t *fifo, void *element)
{
	//Parameter check
	if (fifo == NULL || element == NULL)
	{
		return -1;
	}

	//Check if the length is less than capacity
	if (fifo->length < fifo->capacity)
	{
		fifo->last = fifo->last->nxt;
		fifo->last->element = element;
		fifo->length += 1;
	}
	else
	{

	}
}

void *llfifo_dequeue(llfifo_t *fifo)
{

}


int llfifo_length(llfifo_t *fifo)
{

}


int llfifo_capacity(llfifo_t *fifo)
{

}


void llfifo_destroy(llfifo_t *fifo)
{

}
