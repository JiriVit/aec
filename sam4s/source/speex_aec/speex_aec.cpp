#include <cross_studio_io.h>

#include "speexdsp_types.h"
#include "arch.h"
#include "pseudofloat.h"

#include "speex_aec.h"

/** Speex echo cancellation state. */
struct SpeexEchoState_ {
   int frame_size;           /**< Number of samples processed each time */
   int window_size;
   int M;
   int cancel_count;
   int adapted;
   int saturated;
   int screwed_up;
   int C;                    /** Number of input channels (microphones) */
   int K;                    /** Number of output channels (loudspeakers) */
   spx_int32_t sampling_rate;
   spx_word16_t spec_average;
   spx_word16_t beta0;
   spx_word16_t beta_max;
   spx_word32_t sum_adapt;
   spx_word16_t leak_estimate;

   spx_word16_t *e;      /* scratch */
   spx_word16_t *x;      /* Far-end input buffer (2N) */
   spx_word16_t *X;      /* Far-end buffer (M+1 frames) in frequency domain */
   spx_word16_t *input;  /* scratch */
   spx_word16_t *y;      /* scratch */
   spx_word16_t *last_y;
   spx_word16_t *Y;      /* scratch */
   spx_word16_t *E;
   spx_word32_t *PHI;    /* scratch */
   spx_word32_t *W;      /* (Background) filter weights */
#ifdef TWO_PATH
   spx_word16_t *foreground; /* Foreground filter weights */
   spx_word32_t  Davg1;  /* 1st recursive average of the residual power difference */
   spx_word32_t  Davg2;  /* 2nd recursive average of the residual power difference */
   spx_float_t   Dvar1;  /* Estimated variance of 1st estimator */
   spx_float_t   Dvar2;  /* Estimated variance of 2nd estimator */
#endif
   spx_word32_t *power;  /* Power of the far-end signal */
   spx_float_t  *power_1;/* Inverse power of far-end */
   spx_word16_t *wtmp;   /* scratch */
#ifdef FIXED_POINT
   spx_word16_t *wtmp2;  /* scratch */
#endif
   spx_word32_t *Rf;     /* scratch */
   spx_word32_t *Yf;     /* scratch */
   spx_word32_t *Xf;     /* scratch */
   spx_word32_t *Eh;
   spx_word32_t *Yh;
   spx_float_t   Pey;
   spx_float_t   Pyy;
   spx_word16_t *window;
   spx_word16_t *prop;
   void *fft_table;
   spx_word16_t *memX, *memD, *memE;
   spx_word16_t preemph;
   spx_word16_t notch_radius;
   spx_mem_t *notch_mem;

   /* NOTE: If you only use speex_echo_cancel() and want to save some memory, remove this */
   spx_int16_t *play_buf;
   int play_buf_pos;
   int play_buf_started;
};


int speex_aec_test(void)
{
	return 4;
}
