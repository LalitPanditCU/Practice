/*
 * cmd.h
 *
 *  Created on: Apr 22, 2022
 *      Author: lpandit
 */

#ifndef CMD_H_
#define CMD_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void get_cmd(char *cmd, size_t size);
void process_cmd(size_t count, const char **tokens);
size_t get_tokens(char *sptr, char **tokens);

#endif /* CMD_H_ */
