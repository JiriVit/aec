#include <stdio.h>
#include <string.h>
#include <cross_studio_io.h>

#include "fft_test.h"
#include "speex_aec.h"
#include "utils.h"

int main(void)
{
	init();

	speex_aec_test();

	while(1);
}
