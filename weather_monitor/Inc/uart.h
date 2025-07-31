/*
 * uart.h
 *
 *  Created on: Jun 6, 2025
 *      Author: sunbeam
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART2_Init(void);
void UART2_SendChar(char c);
void UART2_SendString(const char *str);
void UART2_SendNumber(uint32_t num);

#endif // UART_H
