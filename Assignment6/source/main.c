/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include "sysclock.h"

#include "uart.h"


int main(void)
{
  sysclock_init();
  uint8_t c;

  // TODO: initialize the UART here
  UART0_init();

  //Start the loop
  while (1)
  {
	  c = UART0_Receive_Poll();
	  UART0_Transmit_Poll(c+1);
  }


  // enter infinite loop
  while (1) {
    __asm volatile ("nop");
  }

  return 0 ;
}
