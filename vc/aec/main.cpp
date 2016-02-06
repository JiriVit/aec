// aec.cpp : Defines the entry point for the console application.
//

#include <stdint.h>
#include <stdio.h>

#include "stdafx.h"
#include "aec.h"

#define SAMPLE_COUNT 24000

const char *mic_fn = "..\\..\\data\\01_mic_clvl12.raw";
const char *spk_fn = "..\\..\\data\\01_spk_clvl12.raw";
const char *out_fn = "..\\..\\data\\out\\01_out.raw";

int16_t mic[SAMPLE_COUNT];
int16_t spk[SAMPLE_COUNT];
int16_t out[SAMPLE_COUNT];

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *fp;
	AEC aec;
	size_t sampleSize;
	int i;

	/* init */
	sampleSize = sizeof(int16_t);
	memset(out, 0, sizeof(out));

	/* read input data */
	fp = fopen(mic_fn, "rb");
	fread(mic, sampleSize, SAMPLE_COUNT, fp);
	fclose(fp);
	fp = fopen(spk_fn, "rb");
	fread(spk, sampleSize, SAMPLE_COUNT, fp);
	fclose(fp);

	/* calculate echo cancellation */
	for (i = 0; i < SAMPLE_COUNT; i++) {
		out[i] = aec.doAEC(mic[i], spk[i]);
	}

	/* write output data */
	fp = fopen(out_fn, "wb");
	fwrite(out, sampleSize, SAMPLE_COUNT, fp);
	fclose(fp);

	return 0;
}

