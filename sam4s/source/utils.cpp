#include <sam.h>
#include <stdint.h>

#include "utils.h"



/* --- GLOBAL VARIABLES ----------------------------------------------------- */

/** Increased each milisecond. */
uint32_t volatile timer_ms;



/* --- STATIC FUNCTIONS ----------------------------------------------------- */

static void timer_init(void)
{
	timer_ms = 0;
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

static void led_init(void)
{
	PIOC->PIO_PER = PIO_PC1;
	PIOC->PIO_OER = PIO_PC1;
	PIOC->PIO_SODR = PIO_PC1;
}

static void led_invert(void)
{
	if (PIOC->PIO_ODSR & PIO_PC1)
		PIOC->PIO_CODR = PIO_PC1;
	else
		PIOC->PIO_SODR = PIO_PC1;
}

static void power_init(void)
{
	/* charge current 500 mA */
	PIOC->PIO_PER = PIO_PC30;
	PIOC->PIO_OER = PIO_PC30;
	PIOC->PIO_SODR = PIO_PC30;

	/* charge enable */
	PIOC->PIO_PER = PIO_PC29;
	PIOC->PIO_ODR = PIO_PC29;
}



/* --- EXPORTED FUNCTIONS --------------------------------------------------- */

void init(void)
{
	power_init();
	led_init();
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

	if ((timer_ms % 500) == 0)
		led_invert();
}


