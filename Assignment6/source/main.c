/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include "sysclock.h"

#include "cbfifo.h"
#include "uart.h"
#include "cmd.h"


int main(void)
{

  const size_t size = 255;
  char cmd[size];
  char *tokens[size];
  sysclock_init();

  // Initialize the UART here
  cbfifo_init();
  UART0_init();

  //Start the loop
  while (1)
  {
      get_cmd(cmd, size);
      int count = get_tokens(cmd, (char **)tokens);

      process_cmd(count, (const char **)tokens);
  }

  return 0 ;
}
