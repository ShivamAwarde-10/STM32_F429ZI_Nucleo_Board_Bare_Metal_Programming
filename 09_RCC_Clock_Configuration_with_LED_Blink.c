/*
* Project : RCC HSE + PLL Configuration
*
* Pin Configuration:
* PB7 -> LD2 Blue LED Output
*
* Clock Configuration:
*
* HSE = 8 MHz (ST-LINK MCO)
*
* PLLM = 8
* PLLN = 336
* PLLP = 2
* PLLQ = 7
*
* Calculation:
*
* PLL Input
* = HSE / PLLM
* = 8 / 8
* = 1 MHz
*
* VCO Output
* = 1 × 336
* = 336 MHz
*
* SYSCLK
* = 336 / 2
* = 168 MHz
*
* USB Clock
* = 336 / 7
* = 48 MHz
*
* Objective:
* Configure HSE → PLL → SYSCLK
* and blink LD2 (PB7).
*
* LED is blinking much faster because the MCU is now running from the PLL clock (168 MHz)
* instead of the default HSI (16 MHz).
*/


#include <stdint.h>

#define RCC_BASE        0x40023800UL        // RCC base address
#define GPIOB_BASE      0x40020400UL        // GPIOB base address
#define FLASH_BASE      0x40023C00UL        // FLASH base address

#define RCC_CR          *(volatile uint32_t*)(RCC_BASE + 0x00)  // Clock control register
#define RCC_PLLCFGR     *(volatile uint32_t*)(RCC_BASE + 0x04)  // PLL configuration register
#define RCC_CFGR        *(volatile uint32_t*)(RCC_BASE + 0x08)  // Clock configuration register
#define RCC_AHB1ENR     *(volatile uint32_t*)(RCC_BASE + 0x30)  // AHB1 peripheral clock enable

#define FLASH_ACR       *(volatile uint32_t*)(FLASH_BASE + 0x00) // Flash access control register

#define GPIOB_MODER     *(volatile uint32_t*)(GPIOB_BASE + 0x00) // GPIOB mode register
#define GPIOB_ODR       *(volatile uint32_t*)(GPIOB_BASE + 0x14) // GPIOB output data register

void delay(void)
{
    for(volatile uint32_t i = 0; i < 1000000; i++); // Software delay
}

int main(void)
{
    /* Enable HSE oscillator */
    RCC_CR |= (1U << 16);

    /* Wait until HSE becomes ready */
    while(!(RCC_CR & (1U << 17)));

    /* Configure Flash latency for 168 MHz operation */
    FLASH_ACR |= (5U << 0);      // 5 wait states
    FLASH_ACR |= (1U << 8);      // Prefetch enable
    FLASH_ACR |= (1U << 9);      // Instruction cache enable
    FLASH_ACR |= (1U << 10);     // Data cache enable

    /* Configure PLL */
    RCC_PLLCFGR =
            (8U   << 0)  |       // PLLM = 8
            (336U << 6)  |       // PLLN = 336
            (0U   << 16) |       // PLLP = 2
            (1U   << 22) |       // PLL source = HSE
            (7U   << 24);        // PLLQ = 7

    /* Enable PLL */
    RCC_CR |= (1U << 24);

    /* Wait until PLL becomes ready */
    while(!(RCC_CR & (1U << 25)));

    /* Select PLL as system clock */
    RCC_CFGR &= ~(3U << 0);
    RCC_CFGR |=  (2U << 0);

    /* Wait until PLL is used as SYSCLK */
    while(((RCC_CFGR >> 2) & 0x3) != 0x2);

    /* Enable GPIOB peripheral clock */
    RCC_AHB1ENR |= (1U << 1);

    /* Clear PB7 mode bits */
    GPIOB_MODER &= ~(3U << (7 * 2));

    /* Configure PB7 as output */
    GPIOB_MODER |=  (1U << (7 * 2));

    while(1)
    {
        /* Toggle LD2 Blue LED */
        GPIOB_ODR ^= (1U << 7);

        /* Visible delay */
        delay();
    }
}
