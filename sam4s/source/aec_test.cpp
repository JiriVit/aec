#include <arm_math.h>
#include <cross_studio_io.h>
#include "gdp06i_testing.h"
#include "aec.h"

static void sw_aec_test(void);
static void fft_test(void);

void aec_test(void)
{
	fft_test();
}

#define FFT_SIZE 10

float32_t fft_in[] = {
#include "fft_in.inc"	
};

static void fft_test(void)
{
	arm_rfft_fast_instance_f32 rfft;
	float32_t fft_out[FFT_SIZE];

	arm_rfft_fast_init_f32(&rfft, FFT_SIZE);
	memset(fft_out, 0, sizeof(fft_out));

	arm_rfft_fast_f32(&rfft, fft_in, fft_out, 0);

	debug_printf("fft done\n");
}

static void sw_aec_test(void)
{
	int d[] = {0x09ac, 0x09ac, 0x0906, 0x074e, 0x101c, 0x0290, 0x0eb6, 0x0ab2, 0x08cc, 0x064a};
	int x[] = {0x0862, 0x00b8, 0x0602, 0x0252, 0x0160, 0x08b2, 0x0044, 0x00f4, 0x0c6e, 0xf60c};
	int cnt, e;
	uint32_t time;
	AEC aec;

	cnt = 0;
	time = timer_ms;

	while ((timer_ms - time) < 1000) {
		e = aec.doAEC(d[cnt % 10], x[cnt % 10]);
//		e = aec.dtd(d[cnt % 10], x[cnt % 10]);
		cnt++;
	}

	/* cnt musi byt vetsi nez 8000, jinak to nebude fungovat :-) */
	debug_printf("cnt = %d\n", cnt);
}
