/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include <stdio.h>
#include <string.h>
#include "sysclock.h"

#include "cbfifo.h"
#include "uart.h"

#define ALPHA_CH(x)   (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') || ((x) >= '0' && (x) <= '9'))
#define UPPER(x)     (((x) >= 'a') && ((x) <= 'z')) ? x -= 32 : x;

/*
 *
 */
void print_line(const char *line)
{
    printf("\r\n%s\r\n", line);
}

/*
 *
 */
void strupper(const char *s, char *d)
{
      while (*s)
      {
    	  char c = *s++;
    	  *d++ = (c >= 'a' && c <= 'z') ? c -= 32 : c;
      }

      *d = '\0';
}

/*
 *
 */

size_t get_tokens(char *sptr, char **tokens)
{
	size_t i = 0;

	while (*sptr)
	{
		 char ch = *sptr;
		 if (ALPHA_CH(ch))
		 {
			tokens[i++] = sptr;

			do {
				ch = *(++sptr);
			} while (ALPHA_CH(ch));

			if (*sptr) *sptr++ = '\0';
		 }
		 else
		 {
			do {
				ch = *(++sptr);
			} while (!ALPHA_CH(ch) && *sptr);

		 }
	}

	return i;

}

/*
 *
 */
void process_cmd(size_t count, const char **tokens)
{
	char cmd[255];

	if (count == 0)
	{
		printf("\r\n");
		return;
	}

	if (count == 1)
	{
	    strupper(tokens[0], cmd);
		if (!strcmp(cmd, "AUTHOR"))
		{
			print_line("Lalit Pandit");
		}
		else
		{
			printf("\r\nUnknown command: (");
			printf(tokens[0]);
			printf(")\r\n");
		}
	}
	else if (count == 3)
	{

	}
}
/*
 *
 */
void get_cmd(char *cmd, size_t size)
{
		int i = 0;
		uint8_t c;

		i = 0;
		printf("?");
		do
		{
			c = getchar();

			if (c != 255)
			{
			  if (c == '\b' && i != 0)  //Backspace
			  {
				  printf("\b \b");
				  i--;
			  }
			  else
			  {
				  printf("%c",c);
				  cmd[i++] = c;
			  }
			}
		} while (c != '\r' || i == size);

	    if (i != 0)
	    {
	    	  cmd[i-1] = '\0';
	    }
	    else
	    {
	    	cmd[0] = '\0';
	    }
}

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
