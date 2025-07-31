
#ifndef LCD_I2C_H
#define LCD_I2C_H


void LCD_Init(void);
void LCD_SendCmd(int cmd);
void LCD_SendData(int data);
void LCD_SendString(char *str);

#endif /* LCD_I2C_H_ */
