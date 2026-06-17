/*
 * Project : Push Button LED Toggle (Polling Method)
 *
 * Pin Configuration:
 *
 * PC13 ---> USER Push Button (Input)
 * MODER13 = 00 (Input Mode)
 * IDR13 = 1  -> Button Pressed
 * IDR13 = 0  -> Button Released
 *
 * PB7  ---> LD2 Blue LED (Output)
 * MODER7 = 01 (General Purpose Output Mode)
 * ODR7 = 1 -> LED ON
 * ODR7 = 0 -> LED OFF
 *
 * Clock Configuration:
 *
 * RCC_AHB1ENR[1] = 1 -> Enable GPIOB Clock
 * RCC_AHB1ENR[2] = 1 -> Enable GPIOC Clock
 *
 * Logic:
 *
 * Read PC13 continuously (Polling)
 * Button Pressed -> Toggle PB7 LED
 * Wait for Button Release
 * Repeat
 */

#include <stdint.h>                     // Standard integer data types

#define RCC_BASE       0x40023800UL     // RCC peripheral base address
#define GPIOB_BASE     0x40020400UL     // GPIOB peripheral base address
#define GPIOC_BASE     0x40020800UL     // GPIOC peripheral base address

#define RCC_AHB1ENR    *(volatile uint32_t*)(RCC_BASE + 0x30)   // AHB1 clock enable register

#define GPIOB_MODER    *(volatile uint32_t*)(GPIOB_BASE + 0x00) // GPIOB mode register
#define GPIOB_ODR      *(volatile uint32_t*)(GPIOB_BASE + 0x14) // GPIOB output data register

#define GPIOC_MODER    *(volatile uint32_t*)(GPIOC_BASE + 0x00) // GPIOC mode register
#define GPIOC_IDR      *(volatile uint32_t*)(GPIOC_BASE + 0x10) // GPIOC input data register

void delay(void)
{
    for(volatile int i=0;i<200000;i++); // Software delay for debouncing
}

int main(void)
{
    RCC_AHB1ENR |= (1<<1);              // Enable GPIOB clock
    RCC_AHB1ENR |= (1<<2);              // Enable GPIOC clock

    // PB7 Output

    GPIOB_MODER &= ~(3U<<(7*2));        // Clear PB7 mode bits
    GPIOB_MODER |=  (1U<<(7*2));        // Set PB7 as output mode

    // PC13 Input

    GPIOC_MODER &= ~(3U<<(13*2));       // Set PC13 as input mode

    while(1)                            // Infinite loop
    {
        if(GPIOC_IDR & (1<<13))         // Check if push button is pressed
        {
            delay();                    // Debounce button press

            GPIOB_ODR ^= (1<<7);        // Toggle LED state

            while(GPIOC_IDR & (1<<13)); // Wait until button is released
        }
    }
}
