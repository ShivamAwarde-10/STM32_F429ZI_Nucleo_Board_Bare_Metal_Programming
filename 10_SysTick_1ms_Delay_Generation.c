/*
 * Project : SysTick 1ms Delay Generation
 *
 * Pin Configuration:
 *
 * PB7 -> LD2 Blue LED Output
 *
 * SysTick Configuration:
 *
 * System Clock = 16 MHz (HSI)
 *
 * SysTick Clock = CPU Clock
 *
 * Reload Value:
 *
 * 16000000 / 1000
 * = 16000
 *
 * 1 ms Tick Generation
 *
 * Objective:
 * Generate accurate 1 ms delay using SysTick
 * and blink PB7 LED every 500 ms.
 */


#include <stdint.h>                           // Standard fixed-width integer data types

#define RCC_BASE        0x40023800UL          // RCC peripheral base address
#define GPIOB_BASE      0x40020400UL          // GPIOB peripheral base address

#define RCC_AHB1ENR     *(volatile uint32_t*)(RCC_BASE + 0x30)   // AHB1 peripheral clock enable register

#define GPIOB_MODER     *(volatile uint32_t*)(GPIOB_BASE + 0x00) // GPIOB mode register
#define GPIOB_ODR       *(volatile uint32_t*)(GPIOB_BASE + 0x14) // GPIOB output data register

/* SysTick Registers */

#define SYST_CSR        *(volatile uint32_t*)0xE000E010          // SysTick control and status register
#define SYST_RVR        *(volatile uint32_t*)0xE000E014          // SysTick reload value register
#define SYST_CVR        *(volatile uint32_t*)0xE000E018          // SysTick current value register

void SysTick_DelayMs(uint32_t ms)
{
    while(ms--)                           // Repeat delay for required milliseconds
    {
        SYST_RVR = 16000 - 1;             // Load count value for 1ms delay at 16MHz

        SYST_CVR = 0;                     // Clear current counter value

        SYST_CSR = (1<<0) | (1<<2);       // Enable SysTick and select processor clock

        while(!(SYST_CSR & (1<<16)));     // Wait until COUNTFLAG becomes 1

        SYST_CSR = 0;                     // Stop SysTick timer
    }
}

int main(void)
{
    RCC_AHB1ENR |= (1<<1);                // Enable GPIOB peripheral clock

    GPIOB_MODER &= ~(3U<<(7*2));          // Clear PB7 mode bits

    GPIOB_MODER |=  (1U<<(7*2));          // Configure PB7 as output mode

    while(1)                              // Infinite loop
    {
        GPIOB_ODR ^= (1<<7);              // Toggle PB7 LED state

        SysTick_DelayMs(500);             // Wait for 500ms
    }
}




/*
Formula

For 1 ms delay:

Reload Value

= System Clock / 1000

= 16,000,000 / 1000

= 16000

RVR = 15999


For 168 MHz PLL clock:

Reload Value

= 168,000,000 / 1000

= 168000

RVR = 167999
 */

