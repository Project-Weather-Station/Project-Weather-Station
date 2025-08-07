/*
 * adc.c
 *
 *  Created on: Jul 24, 2025
 *      Author: sunbeam
 */

#include "adc.h"
#include "stm32f4xx.h"

void adc_init(void) {
    // Enable GPIOA and ADC1 clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Set PA4 as analog mode (MODER4 = 11)
    GPIOA->MODER |= (3 << (2 * 4));    // Analog mode
    GPIOA->PUPDR &= ~(3 << (2 * 4));   // No pull-up/pull-down

    // ADC Common Control Register: PCLK2 / 4
    ADC->CCR &= ~(3 << 16);            // Clear ADCPRE bits
    ADC->CCR |=  (1 << 16);            // ADCPRE = 01 → PCLK2 / 4

    // Configure ADC1
    ADC1->CR2 = 0;                     // Clear control register
    ADC1->SMPR2 |= (7 << 12);          // Channel 4 sample time = 480 cycles (recommended for stable reads)
    ADC1->SQR3 = 4;                    // First conversion in regular sequence: channel 4 (PA4)
    ADC1->CR2 |= ADC_CR2_ADON;        // Enable ADC1
}

uint16_t adc_read(uint8_t channel) {
    ADC1->SQR3 = channel;             // Select desired channel (e.g., 4 for PA4)
    ADC1->CR2 |= ADC_CR2_SWSTART;     // Start conversion
    while (!(ADC1->SR & ADC_SR_EOC)); // Wait for conversion complete
    return ADC1->DR;                  // Read result
}
