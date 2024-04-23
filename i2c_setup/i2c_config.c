#include<stm32f10x.h>


void i2c_Config(void)
{
	//1. Enable the I2C CLOCK and GPIO CLOCK
	RCC->APB1ENR |= (1<<22); //enable clock i2c
	RCC->APB2ENR |= (1 << 3); // enable clock for gpio port B (pin PB11(SDA) và PB10(SCL))
	//
	
	
}
