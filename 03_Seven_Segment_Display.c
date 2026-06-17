// SSD Connections:
// PC0->a, PC1->b, PC2->c, PC3->d,
// PC4->e, PC5->f, PC6->g, PC7->dp

#include <stdint.h>


#define RCC_BASE 0x40023800UL              // Base address of RCC peripheral

#define GPIOC_BASE 0x40020800UL            // Base address of GPIOC peripheral

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30)      // RCC AHB1 Peripheral Clock Enable Register

#define GPIOC_MODER *(volatile uint32_t*)(GPIOC_BASE + 0x00)    // GPIOC Mode Register

#define GPIOC_ODR   *(volatile uint32_t*)(GPIOC_BASE + 0x14)    // GPIOC Output Data Register

// Seven-segment codes for digits 0 to 9 (Common Cathode SSD)
// Bit pattern corresponds to segments a,b,c,d,e,f,g
int seg[10] =
{
    0x3F,  // Display 0
    0x06,  // Display 1
    0x5B,  // Display 2
    0x4F,  // Display 3
    0x66,  // Display 4
    0x6D,  // Display 5
    0x7D,  // Display 6
    0x07,  // Display 7
    0x7F,  // Display 8
    0x6F   // Display 9
};

int main(void)
{
	/*  Enable GPIOC peripheral clock
       Bit 2 of RCC_AHB1ENR corresponds to GPIOC */

	RCC_AHB1ENR = RCC_AHB1ENR | 4;

    /* Clear mode bits of PC0 to PC7
       Each pin uses 2 bits in MODER register
       0xFFFF = 16 bits -> covers PC0 to PC7 */

	GPIOC_MODER = GPIOC_MODER & ~(0xFFFF << 0);

    /* Configure PC0 to PC7 as General Purpose Output mode
       Output mode = 01 for each pin
       0x1555 = 0001 0101 0101 0101 */

	GPIOC_MODER = GPIOC_MODER | (0x1555 << 0);

	while(1)
	{
		for(int i = 0; i < 10; i++)               // Display digits 0-9 on SSD
		{
			GPIOC_ODR = seg[i];                   // Output SSD pattern for current digit

			for(int j = 0; j <= 1000000; j++);    // Software delay for visibility
		}
	}

    // Never reached
	for(;;);
}
