#include <stdio.h>
#include <string.h>
#include <cross_studio_io.h>

#include "fft_test.h"
#include "utils.h"

int main(void)
{
	init();

	fft_test();

	while(1);
}
