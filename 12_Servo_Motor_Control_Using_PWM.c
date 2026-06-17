/*
 * Project : Servo Motor Control using PWM (TIM2)
 *
 * Servo Motor : SG90
 *
 * Pin Configuration:
 *
 * PA0  ---> TIM2_CH1 PWM Output ---> Servo Signal (Orange Wire)
 *
 * Servo Connections:
 *
 * Orange Wire ---> PA0 (PWM Signal)
 * Red Wire    ---> +5V Supply
 * Brown Wire  ---> GND
 *
 * Important:
 * STM32 GND and Servo GND must be common.
 *
 * Timer Configuration:
 *
 * TIM2 Channel 1
 * PWM Frequency = 50Hz
 * PWM Period    = 20ms
 *
 * Pulse Width:
 *
 * 1000us (1ms)   ---> 0°
 * 1500us (1.5ms) ---> 90°
 * 2000us (2ms)   ---> 180°
 */
/*
Your servo continuously moves in this sequence:

0°  ---> Wait
90° ---> Wait
180° ---> Wait
0°  ---> Wait
90° ---> Wait
180° ---> Wait
(repeats forever)
TIM2 generates a 50Hz PWM signal on PA0, and changing CCR1 from 1000 to
2000 varies the pulse width from 1ms to 2ms, rotating the SG90 servo from 0° to 180°.
*/

#include <stdint.h>     // Standard integer data types

/* Base Addresses */

#define RCC_BASE        0x40023800UL         // RCC peripheral base address

#define GPIOA_BASE      0x40020000UL         // GPIOA peripheral base address

#define TIM2_BASE       0x40000000UL         // TIM2 peripheral base address

/* RCC Registers */

#define RCC_AHB1ENR     *(volatile uint32_t*)(RCC_BASE + 0x30)      // AHB1 peripheral clock enable register

#define RCC_APB1ENR     *(volatile uint32_t*)(RCC_BASE + 0x40)      // APB1 peripheral clock enable register

/* GPIO Registers */

#define GPIOA_MODER     *(volatile uint32_t*)(GPIOA_BASE + 0x00)    // GPIOA mode register

#define GPIOA_AFRL      *(volatile uint32_t*)(GPIOA_BASE + 0x20)    // GPIOA alternate function low register

/* TIM2 Registers */

#define TIM2_CR1        *(volatile uint32_t*)(TIM2_BASE + 0x00)     // Timer control register

#define TIM2_EGR        *(volatile uint32_t*)(TIM2_BASE + 0x14)     // Event generation register

#define TIM2_CCMR1      *(volatile uint32_t*)(TIM2_BASE + 0x18)     // Capture compare mode register

#define TIM2_CCER       *(volatile uint32_t*)(TIM2_BASE + 0x20)     // Capture compare enable register

#define TIM2_CNT        *(volatile uint32_t*)(TIM2_BASE + 0x24)     // Counter register

#define TIM2_PSC        *(volatile uint32_t*)(TIM2_BASE + 0x28)     // Prescaler register

#define TIM2_ARR        *(volatile uint32_t*)(TIM2_BASE + 0x2C)     // Auto reload register

#define TIM2_CCR1       *(volatile uint32_t*)(TIM2_BASE + 0x34)     // Capture compare register 1

void delay(void)     // Software delay function
{
    for(volatile uint32_t i=0;i<3000000;i++);  // Simple blocking delay
}

int main(void)
{
    RCC_AHB1ENR |= (1 << 0);          // Enable GPIOA peripheral clock

    RCC_APB1ENR |= (1 << 0);          // Enable TIM2 peripheral clock

    GPIOA_MODER &= ~(3U << (0 * 2));   // Clear PA0 mode bits

    GPIOA_MODER |=  (2U << (0 * 2));   // Configure PA0 as Alternate Function mode

    GPIOA_AFRL &= ~(0xF << 0);         // Clear alternate function bits for PA0

    GPIOA_AFRL |=  (1 << 0);           // Select AF1 (TIM2_CH1) for PA0

    // Set timer clock to 1 MHz
    // 16MHz / 16 = 1MHz
    TIM2_PSC = 16 - 1;

    // Set PWM period to 20ms
    // 20000 counts @ 1MHz
    TIM2_ARR = 20000 - 1;

    // Initial servo position = 90°
    TIM2_CCR1 = 1500;

    // Clear PWM mode bits
    TIM2_CCMR1 &= ~(7 << 4);

    // Select PWM Mode 1
    TIM2_CCMR1 |=  (6 << 4);

    // Enable preload for CCR1
    TIM2_CCMR1 |= (1 << 3);

    // Enable TIM2 Channel 1 output
    TIM2_CCER |= (1 << 0);

    // Enable auto-reload preload
    TIM2_CR1 |= (1 << 7);

    // Generate update event
    TIM2_EGR |= (1 << 0);

    // Start TIM2 counter
    TIM2_CR1 |= (1 << 0);

    // Infinite loop
    while(1)
    {
        // Move servo to 0°
        TIM2_CCR1 = 1000;

        // Wait
        delay();

        // Move servo to 90°
        TIM2_CCR1 = 1500;

        // Wait
        delay();

        // Move servo to 180°
        TIM2_CCR1 = 2000;

        // Wait
        delay();
    }
}

