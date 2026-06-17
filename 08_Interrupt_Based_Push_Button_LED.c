/*
 * Project : Push Button LED Toggle (Interrupt Method)
 *
 * Pin Configuration:
 *
 * PC13 ---> USER Push Button (Input)
 * MODER13 = 00 (Input Mode)
 * EXTI13 connected to Port C
 * Rising Edge Trigger Enabled
 *
 * PB7 ---> LD2 Blue LED (Output)
 * MODER7 = 01 (General Purpose Output Mode)
 * ODR7 = 1 -> LED ON
 * ODR7 = 0 -> LED OFF
 *
 * Clock Configuration:
 *
 * RCC_AHB1ENR[1]  = 1 -> Enable GPIOB Clock
 * RCC_AHB1ENR[2]  = 1 -> Enable GPIOC Clock
 * RCC_APB2ENR[14] = 1 -> Enable SYSCFG Clock
 *
 * Interrupt Configuration:
 *
 * EXTI13  -> Connected to PC13
 * NVIC IRQ40 -> EXTI15_10 Interrupt
 *
 * Logic:
 *
 * Button Press
 *      ↓
 * EXTI13 Interrupt Generated
 *      ↓
 * ISR Executes
 *      ↓
 * PB7 LED Toggles
 */

#include <stdint.h>                           // Standard integer data types

#define RCC_BASE            0x40023800UL      // RCC peripheral base address
#define GPIOB_BASE          0x40020400UL      // GPIOB peripheral base address
#define GPIOC_BASE          0x40020800UL      // GPIOC peripheral base address
#define SYSCFG_BASE         0x40013800UL      // SYSCFG peripheral base address
#define EXTI_BASE           0x40013C00UL      // EXTI peripheral base address

#define RCC_AHB1ENR         *(volatile uint32_t*)(RCC_BASE + 0x30) // AHB1 clock enable register
#define RCC_APB2ENR         *(volatile uint32_t*)(RCC_BASE + 0x44) // APB2 clock enable register

#define GPIOB_MODER         *(volatile uint32_t*)(GPIOB_BASE + 0x00) // GPIOB mode register
#define GPIOB_ODR           *(volatile uint32_t*)(GPIOB_BASE + 0x14) // GPIOB output data register

#define GPIOC_MODER         *(volatile uint32_t*)(GPIOC_BASE + 0x00) // GPIOC mode register

#define SYSCFG_EXTICR4      *(volatile uint32_t*)(SYSCFG_BASE + 0x14) // EXTI source selection register

#define EXTI_IMR            *(volatile uint32_t*)(EXTI_BASE + 0x00) // Interrupt mask register
#define EXTI_RTSR           *(volatile uint32_t*)(EXTI_BASE + 0x08) // Rising trigger selection register
#define EXTI_PR             *(volatile uint32_t*)(EXTI_BASE + 0x14) // Pending register

#define NVIC_ISER1          *(volatile uint32_t*)0xE000E104        // NVIC interrupt enable register

int main(void)
{
    RCC_AHB1ENR |= (1<<1);                 // Enable GPIOB clock
    RCC_AHB1ENR |= (1<<2);                 // Enable GPIOC clock

    RCC_APB2ENR |= (1<<14);                // Enable SYSCFG clock

    GPIOB_MODER &= ~(3U<<(7*2));           // Clear PB7 mode bits
    GPIOB_MODER |=  (1U<<(7*2));           // Configure PB7 as output

    GPIOC_MODER &= ~(3U<<(13*2));          // Configure PC13 as input

    SYSCFG_EXTICR4 &= ~(0xF << 4);         // Clear EXTI13 source selection
    SYSCFG_EXTICR4 |=  (0x2 << 4);         // Connect EXTI13 to Port C

    EXTI_IMR |= (1<<13);                   // Unmask EXTI13 interrupt

    EXTI_RTSR |= (1<<13);                  // Enable rising edge trigger

    NVIC_ISER1 |= (1<<(40-32));            // Enable EXTI15_10 IRQ in NVIC

    while(1)
    {
    }                                      // Wait for interrupt
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_PR & (1<<13))                  // Check if EXTI13 caused interrupt
    {
        GPIOB_ODR ^= (1<<7);               // Toggle PB7 LED

        EXTI_PR |= (1<<13);                // Clear EXTI13 pending flag
    }
}
