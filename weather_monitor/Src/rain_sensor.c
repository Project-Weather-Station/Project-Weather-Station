/*
 * rain_sensor.c
 *
 *  Created on: Jun 6, 2025
 *      Author: sunbeam
 */


#include "rain_sensor.h"
#include "stm32f4xx.h"

// Definitions
#define DIGITAL_SENSOR_PORT GPIOC
#define DIGITAL_SENSOR_PIN  13

void RainSensor_Init(void)
{
    // ----- Digital Input Init (PC13) -----
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // Set PC13 as input (00)
    DIGITAL_SENSOR_PORT->MODER &= ~(0x3 << (DIGITAL_SENSOR_PIN * 2));

    // Optional: Pull-up
    DIGITAL_SENSOR_PORT->PUPDR &= ~(0x3 << (DIGITAL_SENSOR_PIN * 2));
    DIGITAL_SENSOR_PORT->PUPDR |=  (0x1 << (DIGITAL_SENSOR_PIN * 2)); // Pull-up

    // ----- Analog Input Init (PA0 / ADC1_IN0) -----
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    GPIOA->MODER |= (0x3 << (0 * 2));  // Analog mode for PA0
    GPIOA->PUPDR &= ~(0x3 << (0 * 2)); // No pull-up/down

    // ADC Configuration
    ADC1->CR2 = 0;
    ADC1->SQR3 = 0; // Channel 0 (PA0)
    ADC1->SMPR2 |= ADC_SMPR2_SMP0_2; // Sample time = 144 cycles
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC1
}

uint8_t RainSensor_ReadDigital(void)
{
    return (DIGITAL_SENSOR_PORT->IDR & (1 << DIGITAL_SENSOR_PIN)) ? 1 : 0;
}

uint16_t RainSensor_ReadAnalog(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}
