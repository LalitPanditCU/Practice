/*
 * main.c
 *
 *  Created on: Mar 29, 2022
 *      Author: lpandit
 */

#include "test_cbfifo.h"
#include "test_llfifo.h"

int main(int argc, char *argv[])
{
	test_llfifo();

	test_cbfifo();
}
