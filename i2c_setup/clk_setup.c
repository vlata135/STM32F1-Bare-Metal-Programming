#include<stm32f10x.h>

/*
 * For Configuring the clock settings in STM32 blue chip board we need to do following things
 * 1) Enable the HSE and wait for HSE to become ready.
 * 2) Configure Flash PreFetch and the latency related settings.
 * 3) Configure the Clock for Buses(AHB,APB1,APB2).
 * 4) Configure the PLL(multiplier,HSE divider).
 * 5) Enable the PLL source and wait for it to become stable
 * 6) Enable the Clock source and wait for it to be set
 */

void MCO_pin_conf()
{
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
				 GPIOA->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_CNF8);

				 //CONFIGURE GPIO PIN MODE AS OUTPUT MAX SPEED 50MHZ
				 GPIOA->CRH |= ( GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0);

				 //CONFIGURE GPIO OUTPUT MODE AS alternate function push-pull
				 GPIOA->CRH |= ((GPIO_CRH_CNF8_1) | ~(GPIO_CRH_CNF8_0));
}

void system_clk()
{
	// System_clk will be 32MHz
	
	
	//ENABLE HSE & WAIT FOR HSE TO BECOME READY
	RCC->CR |= RCC_CR_HSEON; // HSE is 8MHz
	while(!(RCC->CR & RCC_CR_HSERDY));
	
	//configure flash prefetch and the latency related
		//this code same as
//	FLASH->ACR |= (1<<0); // latency setup: 001 One wait state, if 24 MHz <= SYSCLK <= 48 MHz
//	FLASH->ACR |= (1<<4); // Prefetch buffer enable: 1: Prefetch is enabled
	FLASH->ACR |= FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_1;
	
	// CONFIGURE PLL AND Buses(AHB,APB1,APB2)
	//PLL SOURCE: HERE  HSE IS USED AS SOURCE
	RCC->CFGR |= RCC_CFGR_PLLSRC;
	//HSE DEVIDER FOR PLL(IF HSE IS USED AS SOURCE FOR PLL) : NOT DIVIDING CLOCK
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE ;  // SO INPUT TO PLL IS 8MHZ
	//PLL MULTIPLIER: HERE I AM MULTIPLYING HSE OUTPUT WITH 4
	RCC->CFGR |= RCC_CFGR_PLLMULL4; //SO OUTPUT FROM PLL IS 32MHZ
	//BUS CLOCK CONFIGURE(APB1,APB2,AHB): NOT DIVIDING
	RCC->CFGR |= (RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_HPRE_DIV1);  // all buses runs at 32MHZ
	
	//ENABLE THE PLL
	RCC->CR |= RCC_CR_PLLON;
	//WAIT FOR PLL TO SET
	while(!(RCC->CR & RCC_CR_PLLRDY));
	
	//ENABLE SYSTEMCLK AND WAIT
	 RCC->CFGR |= RCC_CFGR_SW_PLL;
	 while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
	 
	  MCO_pin_conf();
	 //CLOCK OUTPUT ON MCO PIN
	 RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;
	 
}

