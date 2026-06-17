#include <stdint.h>
#include <stdio.h>

int delay()
{
	for(int i=0; i<150000; i++);
}

#define RCC_BASE 0x40023800UL

#define GPIOA_BASE 0x40020000UL

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30)

#define GPIOA_MODER *(volatile uint32_t*)(GPIOA_BASE + 0x00)
#define GPIOA_ODR *(volatile uint32_t*)(GPIOA_BASE + 0x14)
#define GPIOA_IDR *(volatile uint32_t*)(GPIOA_BASE + 0x10)
#define GPIOA_PUPDR *(volatile uint32_t*)(GPIOA_BASE + 0x0C)

int main(void)
{
	RCC_AHB1ENR = RCC_AHB1ENR | 1;

	GPIOA_MODER = GPIOA_MODER & ~(0xFF<<12); // PA6, PA7, PA8, PA9 as an INPUT

	GPIOA_MODER = GPIOA_MODER & ~(0xFF<<0);  // clearing the bit
	GPIOA_MODER = GPIOA_MODER | (0x55<<0);   // Output declare PA0,PA1,PA2 & PA3

	GPIOA_PUPDR = GPIOA_PUPDR & ~(0xFF<<12);
	GPIOA_PUPDR = GPIOA_PUPDR | (0x55<<12);

	while(1)
	{
		GPIOA_ODR = GPIOA_ODR | (0x0F<<0);  //PA0, PA1 PA2 and PA3 all are HIGH

		GPIOA_ODR = GPIOA_ODR & ~(1<<0);    //PA0 (R1) row is LOW

		// Scanning ths columns

		if(!(GPIOA_IDR & (1<<6)))
		{
			printf("1\n");
			delay();
		}
		if(!(GPIOA_IDR & (1<<7)))
		{
			printf("2\n");
			delay();
		}
		if(!(GPIOA_IDR & (1<<8)))
		{
		    printf("3\n");
		    delay();
		}
		if(!(GPIOA_IDR & (1<<9)))
		{
			printf("A\n");
			delay();
		}

		GPIOA_ODR = GPIOA_ODR | (0x0F<<0);  //PA0, PA1 PA2 and PA3 all are HIGH

	    GPIOA_ODR = GPIOA_ODR & ~(1<<1);    //PA1 (R2) row is LOW

	    // Scanning ths columns

	    if(!(GPIOA_IDR & (1<<6)))
	    {
	    	printf("4\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<7)))
	    {
	    	printf("5\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<8)))
	    {
	    	printf("6\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<9)))
	    {
	    	printf("B\n");
	    	delay();
	    }

	    GPIOA_ODR = GPIOA_ODR | (0x0F<<0);  //PA0, PA1 PA2 and PA3 all are HIGH

	    GPIOA_ODR = GPIOA_ODR & ~(1<<2);    //PA2 (R3) row is LOW

	    // Scanning ths columns

	    if(!(GPIOA_IDR & (1<<6)))
	    {
	    	printf("7\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<7)))
	    {
	    	printf("8\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<8)))
	    {
	    	printf("9\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<9)))
	    {
	    	printf("C\n");
	    	delay();
	    }

	    GPIOA_ODR = GPIOA_ODR | (0x0F<<0);  //PA0, PA1 PA2 and PA3 all are HIGH

	    GPIOA_ODR = GPIOA_ODR & ~(1<<3);    //PA3 (R4) row is LOW

	    // Scanning ths columns

	    if(!(GPIOA_IDR & (1<<6)))
	    {
	    	printf("*\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<7)))
	    {
	    	printf("0\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<8)))
	    {
	    	printf("#\n");
	    	delay();
	    }
	    if(!(GPIOA_IDR & (1<<9)))
	    {
	    	printf("D\n");
	    	delay();
	    }
	}

	for(;;);
}
