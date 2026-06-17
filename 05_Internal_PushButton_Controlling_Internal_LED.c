#include <stdint.h>
#include <stdio.h>

/* RCC Base Address */
#define RCC_BASE        0x40023800UL

/* GPIO Base Addresses */
#define GPIOB_BASE      0x40020400UL
#define GPIOC_BASE      0x40020800UL

/* RCC Register */
#define RCC_AHB1ENR     *(volatile uint32_t*)(RCC_BASE + 0x30)

/* GPIOB Registers */
#define GPIOB_MODER     *(volatile uint32_t*)(GPIOB_BASE + 0x00)
#define GPIOB_ODR       *(volatile uint32_t*)(GPIOB_BASE + 0x14)

/* GPIOC Registers */
#define GPIOC_MODER     *(volatile uint32_t*)(GPIOC_BASE + 0x00)
#define GPIOC_IDR       *(volatile uint32_t*)(GPIOC_BASE + 0x10)

int main(void)
{
    RCC_AHB1ENR |= (1U << 1);               /* Enable GPIOB Clock */

    RCC_AHB1ENR |= (1U << 2);               /* Enable GPIOC Clock */

    GPIOB_MODER &= ~(3U << (7 * 2));        /* Configure PB7 as Output */
    GPIOB_MODER |=  (1U << (7 * 2));

    GPIOC_MODER &= ~(3U << (13 * 2));       /* Configure PC13 as Input */

    while(1)
    {
        /* Read USER Button State */

        if(GPIOC_IDR & (1U << 13))
        {
            GPIOB_ODR |= (1U << 7);             /* Button Pressed -> LD2 ON */
        }
        else
        {
            GPIOB_ODR &= ~(1U << 7);            /* Button Released -> LD2 OFF */
        }
    }

    return 0;
}
