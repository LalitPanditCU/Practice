/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */

#include "sysclock.h"


int main(void)
{
  sysclock_init();

  // TODO: initialize the UART here


  // enter infinite loop
  while (1) {
    __asm volatile ("nop");
  }

  return 0 ;
}
