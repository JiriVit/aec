#include <arm_math.h>
#include <cross_studio_io.h>

#include "utils.h"

#include "fft_test.h"



/* --- MACROS --------------------------------------------------------------- */

#define FFT_SIZE 128



/* --- LOCAL VARIABLES ------------------------------------------------------ */

/** Instance structure for RFFT. */
static arm_rfft_fast_instance_f32 rfft;

/** FFT input data. */
static float32_t fft_in[] = {
#include "fft_in_128.txt"
};

/** FFT output data. */
static float32_t fft_out[FFT_SIZE];



/* --- STATIC FUNCTIONS ----------------------------------------------------- */

/** Writes FFT output into a text file. */
static void fft_write_output(void)
{
	DEBUG_FILE *df;
	int i;

	df = debug_fopen("fft_out.txt", "wt");
	for (i = 0; i < FFT_SIZE; i++)
		debug_fprintf(df, "%f\n", fft_out[i]);
	debug_fclose(df);
}

/** Performs one FFT calculation and writes output into a text file. */
static void fft_test_and_output(void)
{
	arm_rfft_fast_init_f32(&rfft, FFT_SIZE);
	memset(fft_out, 0, sizeof(fft_out));

	arm_rfft_fast_f32(&rfft, fft_in, fft_out, 0);
	fft_write_output();
	debug_printf("fft done - output written to file\n");
}

/** Measures count of FFT calculations in one second. */
static void fft_measure_rate(void)
{
	uint32_t time, count;
	
	arm_rfft_fast_init_f32(&rfft, FFT_SIZE);
	memset(fft_out, 0, sizeof(fft_out));

	count = 0;
	time = timer_ms;
	while ((timer_ms - time) < 1000) {
		arm_rfft_fast_f32(&rfft, fft_in, fft_out, 0);
		count++;
	}
	
	debug_printf("fft done - calculations per sec: %d\n", count);
}



/* --- EXPORTED FUNCTIONS --------------------------------------------------- */

/** Performs FFT calculation test. */
void fft_test(void)
{
	fft_measure_rate();
}