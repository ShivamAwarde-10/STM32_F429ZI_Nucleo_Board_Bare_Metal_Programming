#include <stdint.h>

#define RCC_BASE 0x40023800UL

#define GPIOB_BASE 0x40020400UL

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30)

#define GPIOB_MODER *(volatile uint32_t*)(GPIOB_BASE + 0x00)
#define GPIOB_ODR *(volatile uint32_t*)(GPIOB_BASE + 0x14)

int main(void)
{
	/*
	 * User LD1: a green user LED is connected to the STM32 I/O PB0 (SB120 ON and SB119
                 OFF) or PA5 (SB119 ON and SB120 OFF) corresponding to the ST Zio D13.
       User LD2: a blue user LED is connected to PB7.
       User LD3: a red user LED is connected to PB14.
	 */

	RCC_AHB1ENR |= (1U << 1);       // Enable GPIOB clock

	GPIOB_MODER &= ~(3U << (0 * 2));
	GPIOB_MODER |=  (1U << (0 * 2));

	GPIOB_ODR |= (1U << 0);

	GPIOB_MODER &= ~(3U << (7*2));
    GPIOB_MODER |=  (1U << (7*2));  // PB7 output

    GPIOB_ODR |= (1U << 7);         // LED ON

	GPIOB_MODER &= ~(3U << (14*2));
	GPIOB_MODER |=  (1U << (14*2)); // PB14 output

	GPIOB_ODR |= (1U << 14);        // LED ON

	for(;;);
}
