#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define GPIOB_BASE 0x40020400UL

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30)

#define GPIOB_MODER *(volatile uint32_t*)(GPIOB_BASE + 0x00)
#define GPIOB_ODR   *(volatile uint32_t*)(GPIOB_BASE + 0x14)

void delay(void)
{
    for(volatile uint32_t i = 0; i < 1000000; i++);
}

int main(void)
{
    // Enable GPIOB clock
    RCC_AHB1ENR |= (1U << 1);

    // PB0 (LD1) output
    GPIOB_MODER &= ~(3U << (0 * 2));
    GPIOB_MODER |=  (1U << (0 * 2));

    // PB7 (LD2) output
    GPIOB_MODER &= ~(3U << (7 * 2));
    GPIOB_MODER |=  (1U << (7 * 2));

    // PB14 (LD3) output
    GPIOB_MODER &= ~(3U << (14 * 2));
    GPIOB_MODER |=  (1U << (14 * 2));

    while(1)
    {
        // Toggle all LEDs
        GPIOB_ODR ^= (1U << 0);   // LD1
        GPIOB_ODR ^= (1U << 7);   // LD2
        GPIOB_ODR ^= (1U << 14);  // LD3

        delay();
    }
}
