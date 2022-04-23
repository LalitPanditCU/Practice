/*
 * cmd.c
 *
 *  Created on: Apr 22, 2022
 *      Author: lpandit
 */
#include "hexdump.h"
#include "cmd.h"

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
	char dump_str[4096];

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
			return;
		}
	}
	else if (count == 3)
	{
	    strupper(tokens[0], cmd);
		if (!strcmp(cmd, "DUMP"))
		{
			int start = (int)ahextoi(tokens[1]);
			int len = (int)aitoi(tokens[2]);

			if (start != -1 && len != -1 && len <= 640)
			{
				hexdump(dump_str, (size_t)4096, (void *) start, (size_t) len);
				printf(dump_str);
				return;
			}
		}
	}

	printf("\r\nUnknown command: (");
	printf(tokens[0]);
	for (int i=1; i < count; i++)
		printf(" %s", tokens[i]);
	printf(")\r\n");

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

			if (c != 255 && c != '\r')
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

	   cmd[i] = '\0';
}
