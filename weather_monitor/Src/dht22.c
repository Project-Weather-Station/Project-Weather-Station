#include "dht22.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN  1  // PA1

void delay_us(uint32_t us)
{
    SysTick->LOAD = (us * (SystemCoreClock / 1000000)) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5; // Processor clock, no interrupt

    while ((SysTick->CTRL & 0x10000) == 0);
    SysTick->CTRL = 0;
}

static void set_pin_output(void)
{
    DHT11_PORT->MODER &= ~(3 << (DHT11_PIN * 2));
    DHT11_PORT->MODER |=  (1 << (DHT11_PIN * 2)); // Output
}

static void set_pin_input(void)
{
    DHT11_PORT->MODER &= ~(3 << (DHT11_PIN * 2)); // Input
}

void DHT11_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    set_pin_output();
    DHT11_PORT->ODR |= (1 << DHT11_PIN); // Pull high
}

static uint8_t check_response(void)
{
    uint8_t response = 0;
    delay_us(40);

    if (!(DHT11_PORT->IDR & (1 << DHT11_PIN)))
    {
        delay_us(80);
        if (DHT11_PORT->IDR & (1 << DHT11_PIN)) response = 1;
        delay_us(80);
    }
    return response;
}

static uint8_t read_byte(void)
{
    uint8_t i, byte = 0;

    for (i = 0; i < 8; i++)
    {
        while (!(DHT11_PORT->IDR & (1 << DHT11_PIN))); // wait for HIGH
        delay_us(40);
        if (DHT11_PORT->IDR & (1 << DHT11_PIN))
            byte |= (1 << (7 - i));
        while (DHT11_PORT->IDR & (1 << DHT11_PIN)); // wait for LOW
    }
    return byte;
}

uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t hum_int, hum_dec, temp_int, temp_dec, checksum;

    set_pin_output();
    DHT11_PORT->ODR &= ~(1 << DHT11_PIN); // Start signal - pull low
    delay_us(18000); // 18ms
    DHT11_PORT->ODR |= (1 << DHT11_PIN);  // Pull high
    delay_us(30);
    set_pin_input();

    if (!check_response()) return 0;

    hum_int  = read_byte();
    hum_dec  = read_byte();
    temp_int = read_byte();
    temp_dec = read_byte();
    checksum = read_byte();

    if ((hum_int + hum_dec + temp_int + temp_dec) == checksum)
    {
        *temperature = temp_int;
        *humidity = hum_int;
        return 1; // success
    }

    return 0; // checksum failed
}
