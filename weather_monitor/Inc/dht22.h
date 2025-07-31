#ifndef DHT11_H_
#define DHT11_H_

#include "stm32f4xx.h"

void DHT11_Init(void);
uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity);
void delay_us(uint32_t us);

#endif
