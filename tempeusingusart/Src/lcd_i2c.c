
#include "lcd_i2c.h"
#include "i2c.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define LCD_ADDR 0x4E
#define EN 0x04
#define RW 0x02
#define RS 0x01
#define BACKLIGHT 0x08

static void LCD_SendNibble(int nibble, int control) {
    int data = nibble | control | BACKLIGHT;
    I2C1_Write(LCD_ADDR, data | EN);
    I2C1_Write(LCD_ADDR, data);
}

void LCD_SendCmd(int cmd) {
    LCD_SendNibble(cmd & 0xF0, 0);
    LCD_SendNibble((cmd << 4) & 0xF0, 0);
}

void LCD_SendData(int data) {
    LCD_SendNibble(data & 0xF0, RS);
    LCD_SendNibble((data << 4) & 0xF0, RS);
}

void LCD_SendString(char *str) {
    while (*str) LCD_SendData(*str++);
}

void LCD_Init(void) {
    I2C1_Init();
    for (int i = 0; i < 3; i++) {
        LCD_SendNibble(0x30, 0);
    }
    LCD_SendNibble(0x20, 0); // 4-bit mode

    LCD_SendCmd(0x28); // 2 lines
    LCD_SendCmd(0x0C); // Display on
    LCD_SendCmd(0x01); // Clear
}
