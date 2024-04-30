#include"stm32f10x.h"
#include "i2c.h"
 void timer_initialise()
{
	// Start by making sure the timer's 'counter' is off
	TIM2->CR1 &= ~(TIM_CR1_CEN);
	TIM2->SR &= ~(TIM_SR_UIF);

	//RESET THE TIMER2 BUS
	RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM2RST);
	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);


	//Enable Timer2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;


    TIM2-> CR1 |= TIM_CR1_UDIS; // enable this bit so that no register value is updated
/**********************for example 1*******************************/
    //Timer prescaler value
    TIM2->PSC = 32;
    //TIM2->PSC = 0;
    //Timer Auto reload register value
    TIM2->ARR = 0xffff;
    //65535
    //TIM2->ARR = 32;

    TIM2-> CR1 &= ~TIM_CR1_UDIS; // disable this bit so that values assigned from Timer register can move to preload registers

   //Enable the update generation for updating the shadow register with preload register contents with new values
   TIM2->EGR |= TIM_EGR_UG;
   //Clear the UIF flag as that is set when UG bit is set in EGR to update the content of my register
   TIM2->SR &= ~(TIM_SR_UIF);
}
void delay_us(uint16_t us)
{
	TIM2->CR1 |= TIM_CR1_CEN;
	//TIM2-> CR1 |= TIM_CR1_UDIS; // enable this bit so that no register value is updated
	//TIM2->EGR |= TIM_EGR_UG; // SET UG bit to reinitailse the counter register with 0
	TIM2->CNT=0; // initialise counter register
	//while(!(TIM2->SR & (1<<TIM_SR_UIF)));
	//TIM2->SR &= ~(TIM_SR_UIF); //clear the update interrupt flag as this is set when CNT is reinitialsed with 0
	while(TIM2->CNT < us)
	{	//uart_tran_dec(TIM2->CNT);
	}
	//TIM2->CR1 &= ~TIM_CR1_CEN;

}
void delay_ms(uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	delay_us(1000);
}
void delay_sec(uint16_t secs)
{
	for (uint16_t i=0; i<secs; i++)
	delay_ms(1000);
}
void delay()
{
	TIM2->CR1 |= TIM_CR1_CEN;
	while(!(TIM2->SR & TIM_SR_UIF));
	TIM2->SR &= ~(TIM_SR_UIF);
}
//static int l =0;
void delayus(uint16_t us)
{
   int i =0;
   //TIM2->CR1 |= TIM_CR1_CEN;
   while(i++<us)
   {
	   TIM2->CR1 |= TIM_CR1_CEN;
	   	while(!(TIM2->SR & TIM_SR_UIF));
	   	TIM2->SR &= ~(TIM_SR_UIF);
	  // i++;
   }
   //TIM2->CR1 &= ~TIM_CR1_CEN;
}
//static int p=0;
void delayms(uint16_t ms)
{
   int i=0;
   while(i++<ms)
   {
	  delayus(1000);
     // i++;
   }
  // TIM2->CR1 &= ~TIM_CR1_CEN;
}





#define RTC_WADDR 0b11010000   //Slave Write
#define RTC_RADDR 0b11010001   //Slave Read
int main()
{
	
	system_clk();
	timer_initialise();
	master_init();
	
	int Temp_reg = 10;
	int i = 1;
	while(1)
	{
		GPIOC->ODR &= ~GPIO_ODR_ODR13;


		// writing a byte
		i2c_start_condition_w();
		i2c_address_send_w(RTC_WADDR);
		databyte_send(Temp_reg);
		stop_generation();
		i++;
	}
	
	
}
