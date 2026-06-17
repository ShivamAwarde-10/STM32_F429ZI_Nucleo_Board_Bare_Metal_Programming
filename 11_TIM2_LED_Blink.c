/*
 * Project : TIM2 LED Blink
 *
 * Pin Configuration:
 *
 * PB7 -> LD2 Blue LED Output
 *
 * Timer Configuration:
 *
 * TIM2 Clock = 16 MHz
 *
 * PSC = 16000 - 1
 * ARR = 1000 - 1
 *
 * Timer Tick:
 *
 * 16 MHz / 16000
 * = 1000 Hz
 *
 * 1 Tick = 1 ms
 *
 * ARR = 1000
 * = 1000 ms
 * = 1 Second
 *
 * Objective:
 * Generate 1-second delay using TIM2
 * and blink PB7 LED.
 */

#include <stdint.h>                            // Standard fixed-width integer types

#define RCC_BASE        0x40023800UL           // RCC peripheral base address
#define GPIOB_BASE      0x40020400UL           // GPIOB peripheral base address
#define TIM2_BASE       0x40000000UL           // TIM2 peripheral base address

/* RCC Registers */

#define RCC_AHB1ENR     *(volatile uint32_t*)(RCC_BASE + 0x30) // AHB1 clock enable register
#define RCC_APB1ENR     *(volatile uint32_t*)(RCC_BASE + 0x40) // APB1 clock enable register

/* GPIO Registers */

#define GPIOB_MODER     *(volatile uint32_t*)(GPIOB_BASE + 0x00) // GPIOB mode register
#define GPIOB_ODR       *(volatile uint32_t*)(GPIOB_BASE + 0x14) // GPIOB output data register

/* TIM2 Registers */

#define TIM2_CR1        *(volatile uint32_t*)(TIM2_BASE + 0x00) // TIM2 control register
#define TIM2_SR         *(volatile uint32_t*)(TIM2_BASE + 0x10) // TIM2 status register
#define TIM2_PSC        *(volatile uint32_t*)(TIM2_BASE + 0x28) // TIM2 prescaler register
#define TIM2_ARR        *(volatile uint32_t*)(TIM2_BASE + 0x2C) // TIM2 auto-reload register

int main(void)
{
    RCC_AHB1ENR |= (1 << 1);                   // Enable GPIOB peripheral clock

    RCC_APB1ENR |= (1 << 0);                   // Enable TIM2 peripheral clock

    GPIOB_MODER &= ~(3U << (7 * 2));           // Clear PB7 mode bits

    GPIOB_MODER |=  (1U << (7 * 2));           // Configure PB7 as output mode

    TIM2_PSC = 16000 - 1;                      // Divide 16MHz clock to 1kHz

    TIM2_ARR = 1000 - 1;                       // Generate update event every 1000 counts

    TIM2_CR1 |= (1 << 0);                      // Start TIM2 counter

    while(1)                                   // Infinite loop
    {
        while(!(TIM2_SR & (1 << 0)));          // Wait until timer overflow occurs

        TIM2_SR &= ~(1 << 0);                  // Clear update interrupt flag (UIF)

        GPIOB_ODR ^= (1 << 7);                 // Toggle PB7 LED
    }
}
