/* Pin Configuration Explanation
 PC0 -> Configured as Input Pin
 Used to read the external push button status.

 PC1 -> Configured as Output Pin
 Used to control the external LED through a current-limiting resistor(10k). */


#include <stdint.h>
#include <stdio.h>

#define RCC_BASE 0x40023800UL               // Base address of RCC peripheral

#define GPIOC_BASE 0x40020800UL             // Base address of GPIOC peripheral

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30)   // RCC AHB1 Peripheral Clock Enable Register

#define GPIOC_MODER *(volatile uint32_t*)(GPIOC_BASE + 0x00) // GPIOC Mode Register

#define GPIOC_ODR *(volatile uint32_t*)(GPIOC_BASE + 0x14)   // GPIOC Output Data Register

#define GPIOC_IDR *(volatile uint32_t*)(GPIOC_BASE + 0x10)   // GPIOC Input Data Register

int main(void)
{
	RCC_AHB1ENR = RCC_AHB1ENR | 4;            // Enable GPIOC peripheral clock (AHB1 bus, bit 2)

	GPIOC_MODER = GPIOC_MODER & ~(3 << 0);    // Configure PC0 as Input Mode (MODER0 = 00)

	GPIOC_MODER = GPIOC_MODER & ~(3 << 2);    // Clear PC1 mode bits before configuration

	GPIOC_MODER = GPIOC_MODER | (1 << 2);     // Configure PC1 as General Purpose Output Mode (MODER1 = 01)

	while(1)
	{
		uint32_t val = (GPIOC_IDR & 0x1);     // Read PC0 input state (push button)

		if(val > 0)                           // If button is pressed (PC0 = 1)
		{
			GPIOC_ODR = GPIOC_ODR | (1 << 1); // Turn ON LED connected to PC1
		}
		else
		{
			GPIOC_ODR = GPIOC_ODR & ~(1 << 1);   // Turn OFF LED connected to PC1
		}
	}

	for(;;);
}
