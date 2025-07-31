#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include <stdint.h>
#define SOIL_SENSOR_CHANNEL 4  // ADC0

uint16_t read_soil_moisture(void);

#endif
