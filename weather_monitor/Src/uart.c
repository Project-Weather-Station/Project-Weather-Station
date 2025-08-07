/*
 * uart.c
 *
 *  Created on: Jun 6, 2025
 *      Author: sunbeam
 */

#include "uart.h"
#include "stm32f4xx.h"

void UART2_Init(void)
{
    // Enable GPIOA and USART2 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Set PA2 (TX) to alternate function (AF7 for USART2)
    GPIOA->MODER &= ~(0x3 << (2 * 2));
    GPIOA->MODER |=  (0x2 << (2 * 2));    // Alternate function
    GPIOA->AFR[0] |= (0x7 << (2 * 4));    // AF7 = USART2

    // Configure USART2
    USART2->BRR = (16000000 / 115200);      // Assuming 16MHz clock, 9600 baud
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE; // Enable TX and USART
}

void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE)); // Wait for TX buffer empty
    USART2->DR = c;
}

void UART2_SendString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

void UART2_SendNumber(uint32_t num)
{
    char buffer[12];
    int i = 0;

    if (num == 0)
    {
        UART2_SendChar('0');
        return;
    }

    while (num > 0)
    {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i--)
    {
        UART2_SendChar(buffer[i]);
    }
}

