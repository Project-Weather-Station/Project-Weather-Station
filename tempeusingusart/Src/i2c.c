#include "i2c.h"
#include "stm32f4xx.h"

void I2C1_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER |= (2 << (6 * 2)) | (2 << (7 * 2));  // PB6, PB7 AF
    GPIOB->AFR[0] |= (4 << (6 * 4)) | (4 << (7 * 4));
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);
    GPIOB->OSPEEDR |= (3 << (6 * 2)) | (3 << (7 * 2));

    I2C1->CR2 = 16;
    I2C1->CCR = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Write(int addr, int data) {
    while (I2C1->SR2 & I2C_SR2_BUSY);
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
    I2C1->DR = addr;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}
