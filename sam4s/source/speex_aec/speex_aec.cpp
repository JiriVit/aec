#include <cross_studio_io.h>

#include "speexdsp_types.h"
#include "arch.h"
#include "pseudofloat.h"

#include "speex_aec.h"

/* --- TYPE DEFINITION ------------------------------------------------------ */

/** Speex echo cancellation state. */
struct SpeexEchoState {
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



/* --- GLOBAL VARIABLES ----------------------------------------------------- */

SpeexEchoState AEC_State;



/* --- EXPORTED FUNCTIONS ---------------------------------------------------- */

SpeexEchoState *speex_echo_state_init_mc(int frame_size, 
	int filter_length, int nb_mic, int nb_speakers)
{
   int i,N,M, C, K;
   SpeexEchoState *st = &AEC_State;

   st->K = nb_speakers;
   st->C = nb_mic;
   C=st->C;
   K=st->K;
//#ifdef DUMP_ECHO_CANCEL_DATA
//   if (rFile || pFile || oFile)
//      speex_fatal("Opening dump files twice");
//   rFile = fopen("aec_rec.sw", "wb");
//   pFile = fopen("aec_play.sw", "wb");
//   oFile = fopen("aec_out.sw", "wb");
//#endif

   st->frame_size = frame_size;
   st->window_size = 2*frame_size;
   N = st->window_size;
   M = st->M = (filter_length+st->frame_size-1)/frame_size;
   st->cancel_count=0;
   st->sum_adapt = 0;
   st->saturated = 0;
   st->screwed_up = 0;
   /* This is the default sampling rate */
   st->sampling_rate = 8000;
   st->spec_average = DIV32_16(SHL32(EXTEND32(st->frame_size), 15), st->sampling_rate);
   st->beta0 = DIV32_16(SHL32(EXTEND32(st->frame_size), 16), st->sampling_rate);
   st->beta_max = DIV32_16(SHL32(EXTEND32(st->frame_size), 14), st->sampling_rate);
   st->leak_estimate = 0;
//
//   st->fft_table = spx_fft_init(N);
//
//   st->e = (spx_word16_t*)speex_alloc(C*N*sizeof(spx_word16_t));
//   st->x = (spx_word16_t*)speex_alloc(K*N*sizeof(spx_word16_t));
//   st->input = (spx_word16_t*)speex_alloc(C*st->frame_size*sizeof(spx_word16_t));
//   st->y = (spx_word16_t*)speex_alloc(C*N*sizeof(spx_word16_t));
//   st->last_y = (spx_word16_t*)speex_alloc(C*N*sizeof(spx_word16_t));
//   st->Yf = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
//   st->Rf = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
//   st->Xf = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
//   st->Yh = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
//   st->Eh = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
//
//   st->X = (spx_word16_t*)speex_alloc(K*(M+1)*N*sizeof(spx_word16_t));
//   st->Y = (spx_word16_t*)speex_alloc(C*N*sizeof(spx_word16_t));
//   st->E = (spx_word16_t*)speex_alloc(C*N*sizeof(spx_word16_t));
//   st->W = (spx_word32_t*)speex_alloc(C*K*M*N*sizeof(spx_word32_t));
//#ifdef TWO_PATH
//   st->foreground = (spx_word16_t*)speex_alloc(M*N*C*K*sizeof(spx_word16_t));
//#endif
//   st->PHI = (spx_word32_t*)speex_alloc(N*sizeof(spx_word32_t));
//   st->power = (spx_word32_t*)speex_alloc((frame_size+1)*sizeof(spx_word32_t));
//   st->power_1 = (spx_float_t*)speex_alloc((frame_size+1)*sizeof(spx_float_t));
//   st->window = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
//   st->prop = (spx_word16_t*)speex_alloc(M*sizeof(spx_word16_t));
//   st->wtmp = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
//#ifdef FIXED_POINT
//   st->wtmp2 = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
//   for (i=0;i<N>>1;i++)
//   {
//      st->window[i] = (16383-SHL16(spx_cos(DIV32_16(MULT16_16(25736,i<<1),N)),1));
//      st->window[N-i-1] = st->window[i];
//   }
//#else
//   for (i=0;i<N;i++)
//      st->window[i] = .5-.5*cos(2*M_PI*i/N);
//#endif
//   for (i=0;i<=st->frame_size;i++)
//      st->power_1[i] = FLOAT_ONE;
//   for (i=0;i<N*M*K*C;i++)
//      st->W[i] = 0;
//   {
//      spx_word32_t sum = 0;
//      /* Ratio of ~10 between adaptation rate of first and last block */
//      spx_word16_t decay = SHR32(spx_exp(NEG16(DIV32_16(QCONST16(2.4,11),M))),1);
//      st->prop[0] = QCONST16(.7, 15);
//      sum = EXTEND32(st->prop[0]);
//      for (i=1;i<M;i++)
//      {
//         st->prop[i] = MULT16_16_Q15(st->prop[i-1], decay);
//         sum = ADD32(sum, EXTEND32(st->prop[i]));
//      }
//      for (i=M-1;i>=0;i--)
//      {
//         st->prop[i] = DIV32(MULT16_16(QCONST16(.8f,15), st->prop[i]),sum);
//      }
//   }
//
//   st->memX = (spx_word16_t*)speex_alloc(K*sizeof(spx_word16_t));
//   st->memD = (spx_word16_t*)speex_alloc(C*sizeof(spx_word16_t));
//   st->memE = (spx_word16_t*)speex_alloc(C*sizeof(spx_word16_t));
//   st->preemph = QCONST16(.9,15);
//   if (st->sampling_rate<12000)
//      st->notch_radius = QCONST16(.9, 15);
//   else if (st->sampling_rate<24000)
//      st->notch_radius = QCONST16(.982, 15);
//   else
//      st->notch_radius = QCONST16(.992, 15);
//
//   st->notch_mem = (spx_mem_t*)speex_alloc(2*C*sizeof(spx_mem_t));
//   st->adapted = 0;
//   st->Pey = st->Pyy = FLOAT_ONE;
//
//#ifdef TWO_PATH
//   st->Davg1 = st->Davg2 = 0;
//   st->Dvar1 = st->Dvar2 = FLOAT_ZERO;
//#endif
//
//   st->play_buf = (spx_int16_t*)speex_alloc(K*(PLAYBACK_DELAY+1)*st->frame_size*sizeof(spx_int16_t));
//   st->play_buf_pos = PLAYBACK_DELAY*st->frame_size;
//   st->play_buf_started = 0;

   return st;
}


int speex_aec_test(void)
{
	speex_echo_state_init_mc(256, 512, 1, 1);

	debug_printf("%d\n", sizeof(AEC_State));

	return 4;
}
