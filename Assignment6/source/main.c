/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include "sysclock.h"

#include "uart.h"


int main(void)
{
  uint8_t c=0;
  sysclock_init();

  // TODO: initialize the UART here
  UART0_init();

  //Start the loop
  while (1)
  {

	  UART0_Send_String_Poll((uint8_t *)"\n\rRain In spain\n\r");
	  c = UART0_Receive_Poll();
	  UART0_Transmit_Poll(c);
  }

  return 0 ;
}
