/*
 * uart.h
 *
 *  Created on: Apr 17, 2022
 *      Author: lpandit
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void UART0_init();

uint8_t UART0_Receive_Poll(void);
void UART0_Transmit_Poll(uint8_t data);


#endif /* UART_H_ */
