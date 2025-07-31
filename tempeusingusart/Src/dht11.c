
#include "dht11.h"
#include "stm32f4xx.h"

#define DHT_PORT GPIOA
#define DHT_PIN  1

static void delay_us(int us) {
    // Assuming SysTick is 16 MHz
    SysTick->LOAD = us * 16;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while (!(SysTick->CTRL & 0x10000));
    SysTick->CTRL = 0;
}

static void DHT_SetPinOutput(void) {
    DHT_PORT->MODER |= (1 << (DHT_PIN * 2));
}

static void DHT_SetPinInput(void) {
    DHT_PORT->MODER &= ~(3 << (DHT_PIN * 2));
}

static int DHT_ReadBit(void) {
    while (!(DHT_PORT->IDR & (1 << DHT_PIN)));  // Wait for HIGH
    delay_us(40);
    if (DHT_PORT->IDR & (1 << DHT_PIN)) return 1;
    else return 0;
}

int DHT11_Read(int *temperature, int *humidity) {
    int data[5] = {0};

    DHT_SetPinOutput();
    DHT_PORT->ODR &= ~(1 << DHT_PIN);  // Pull low
    delay_us(18000);
    DHT_PORT->ODR |= (1 << DHT_PIN);   // Pull high
    delay_us(20);
    DHT_SetPinInput();

    // Wait for response
    delay_us(80);  // DHT pulls low
    delay_us(80);  // DHT pulls high

    for (int i = 0; i < 40; i++) {
        data[i/8] <<= 1;
        data[i/8] |= DHT_ReadBit();
        while (DHT_PORT->IDR & (1 << DHT_PIN));  // Wait for end of bit
    }

    if (data[4] != (data[0] + data[2])) return 1;

    *humidity = data[0];
    *temperature = data[2];

    return 0;
}

void DHT11_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    DHT_SetPinOutput();
    DHT_PORT->ODR |= (1 << DHT_PIN);
}
