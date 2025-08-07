#include "adc.h"
#include "soil_sensor.h"

#define SOIL_SENSOR_CHANNEL 4 // PA4 -> ADC1_IN0

uint16_t read_soil_moisture(void) {
    return adc_read(SOIL_SENSOR_CHANNEL);
}
