/*
 * rain_sensor.h
 *
 *  Created on: Jun 6, 2025
 *      Author: sunbeam
 */
#ifndef RAIN_SENSOR_H
#define RAIN_SENSOR_H

#include <stdint.h>

// Initialize both digital and analog interfaces
void RainSensor_Init(void);

// Read digital rain sensor value: 1 = rain, 0 = no rain
uint8_t RainSensor_ReadDigital(void);

// Read analog rain sensor value: returns ADC value (0–4095)
uint16_t RainSensor_ReadAnalog(void);

#endif // RAIN_SENSOR_H
