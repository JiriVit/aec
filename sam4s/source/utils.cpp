#include <sam.h>
#include <stdint.h>

#include "utils.h"



/* --- GLOBAL VARIABLES ----------------------------------------------------- */

uint32_t volatile timer_ms;



/* --- STATIC FUNCTIONS ----------------------------------------------------- */

static void timer_init(void)
{
	timer_ms = 0;
	SysTick_Config(SystemCoreClock / 1000);
}



/* --- EXPORTED FUNCTIONS --------------------------------------------------- */

void init(void)
{
	timer_init();
}

void wait_ms(int interval_ms)
{
	uint32_t timestamp_ms;
	
	timestamp_ms = timer_ms;
	while ((timer_ms - timestamp_ms) < interval_ms)
		asm("nop");
}



/* --- INTERRUPT HANDLERS --------------------------------------------------- */

void SysTick_Handler(void)
{
	timer_ms++;
}


