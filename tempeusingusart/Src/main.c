#include "stm32f4xx.h"
#include "dht11.h"
#include "lcd_i2c.h"
#include <stdio.h>

int main(void) {
    char buffer[32];
    int temp, hum;

    LCD_Init();
    DHT11_Init();

    while (1) {
        if (DHT11_Read(&temp, &hum) == 0) {
            LCD_SendCmd(0x80);
            printf(buffer, "Temp: %d C", temp);
            LCD_SendString(buffer);

            LCD_SendCmd(0xC0);
            printf(buffer, "Hum:  %d %%", hum);
            LCD_SendString(buffer);
        } else {
            LCD_SendCmd(0x80);
            LCD_SendString("Read Error");
        }

        for (volatile int i = 0; i < 1000000; i++);
    }
}
