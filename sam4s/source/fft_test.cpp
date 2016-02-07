#include <arm_math.h>
#include <cross_studio_io.h>

#include "fft_test.h"

/* --- MACROS --------------------------------------------------------------- */

#define FFT_SIZE 64



/* --- LOCAL VARIABLES ------------------------------------------------------ */

/** Instance structure for RFFT. */
static arm_rfft_fast_instance_f32 rfft;

/** FFT input data. */
static float32_t fft_in[] = {
#include "fft_in_64.txt"
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



/* --- EXPORTED FUNCTIONS --------------------------------------------------- */

/** Performs FFT calculation test. */
void fft_test(void)
{
	arm_rfft_fast_init_f32(&rfft, FFT_SIZE);
	memset(fft_out, 0, sizeof(fft_out));

	arm_rfft_fast_f32(&rfft, fft_in, fft_out, 0);
	
	debug_printf("fft done\n");
}