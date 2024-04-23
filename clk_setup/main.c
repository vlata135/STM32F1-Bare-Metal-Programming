


#include "stm32f10x.h"

#include "Delay_F103.h"


void GPIO_Config (void)
{
/************** STEPS TO FOLLOW *****************
	1. Enable the GPIOC clock
	2. Configure the Pin as output
************************************************/
	
	RCC->APB2ENR |= (1<<4);  // Enable GPIOC Clock
	
	GPIOC->CRH |= (1<<20);  // PC13 Output mode 10 MHz, push pull
}


int main ()
{
	SystemInit ();
	
	GPIO_Config ();
	
	TIM2_Config ();
	
	
	while (1)
	{
		GPIOC->BSRR |= (1<<13);  // Set the pin PC13
		
		Delay_ms (500);
		
		GPIOC->BSRR |= (1<<29);  // RESET the pin PC13
		
		Delay_ms (500);
		
	}
}
