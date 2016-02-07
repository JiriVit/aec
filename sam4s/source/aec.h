/* aec.h
 *
 * Copyright (C) DFS Deutsche Flugsicherung (2004). All Rights Reserved.
 *
 * Acoustic Echo Cancellation NLMS-pw algorithm
 *
 * Version 1.3 filter created with www.dsptutor.freeuk.com
 */

#ifndef _AEC_H                  /* include only once */

#include <string.h>
#include "arm_math.h"

// use double if your CPU does software-emulation of float
typedef float32_t REAL;

/* dB Values */
const REAL M0dB = 1.0f;
const REAL M3dB = 0.71f;
const REAL M6dB = 0.50f;
const REAL M9dB = 0.35f;
const REAL M12dB = 0.25f;
const REAL M18dB = 0.125f;
const REAL M24dB = 0.063f;

/* dB values for 16bit PCM */
/* MxdB_PCM = 32767 * 10 ^(x / 20) */
const REAL M10dB_PCM = 10362.0f;
const REAL M20dB_PCM = 3277.0f;
const REAL M25dB_PCM = 1843.0f;
const REAL M30dB_PCM = 1026.0f;
const REAL M35dB_PCM = 583.0f;
const REAL M40dB_PCM = 328.0f;
const REAL M45dB_PCM = 184.0f;
const REAL M50dB_PCM = 104.0f;
const REAL M55dB_PCM = 58.0f;
const REAL M60dB_PCM = 33.0f;
const REAL M65dB_PCM = 18.0f;
const REAL M70dB_PCM = 10.0f;
const REAL M75dB_PCM = 6.0f;
const REAL M80dB_PCM = 3.0f;
const REAL M85dB_PCM = 2.0f;
const REAL M90dB_PCM = 1.0f;

const REAL MAXPCM = 32767.0f;

/* Design constants (Change to fine tune the algorithms */

/* The following values are for hardware AEC and studio quality 
 * microphone */

/* maximum NLMS filter length in taps. A longer filter length gives 
 * better Echo Cancellation, but slower convergence speed and
 * needs more CPU power (Order of NLMS is linear) */
#define NLMS_LEN  (40*8)

/* convergence speed. Range: >0 to <1 (0.2 to 0.7). Larger values give
 * more AEC in lower frequencies, but less AEC in higher frequencies. */
const REAL Stepsize = 0.7f;

/* minimum energy in xf. Range: M70dB_PCM to M50dB_PCM. Should be equal
 * to microphone ambient Noise level */
const REAL Min_xf = M75dB_PCM;

/* Double Talk Detector Speaker/Microphone Threshold. Range <=1
 * Large value (M0dB) is good for Single-Talk Echo cancellation, 
 * small value (M12dB) is good for Doulbe-Talk AEC */
const REAL GeigelThreshold = M6dB;

/* Double Talk Detector hangover in taps. Not relevant for Single-Talk 
 * AEC */
const int Thold = 30 * 8;

/* for Non Linear Processor. Range >0 to 1. Large value (M0dB) is good
 * for Double-Talk, small value (M12dB) is good for Single-Talk */
const REAL NLPAttenuation = M12dB;

/* Below this line there are no more design constants */


/* Exponential Smoothing or IIR Infinite Impulse Response Filter */
class IIR_HP {
  REAL x;

public:
   IIR_HP() { x = 0.0f; };
  REAL highpass(REAL in) {
    const REAL a0 = 0.01f;   /* controls Transfer Frequency */
    /* Highpass = Signal - Lowpass. Lowpass = Exponential Smoothing */
    x += a0 * (in - x);
    return in - x;
  };
};


/* 13 taps FIR Finite Impulse Response filter
 * Coefficients calculated with
 * www.dsptutor.freeuk.com/KaiserFilterDesign/KaiserFilterDesign.html
 */
class FIR_HP13 {
  REAL z[14];
  
public:
   FIR_HP13() { memset(this, 0, sizeof(FIR_HP13)); };
  REAL highpass(REAL in) {
    const REAL a[14] = {
      // Kaiser Window FIR Filter, Filter type: High pass
      // Passband: 300.0 - 4000.0 Hz, Order: 12
      // Transition band: 100.0 Hz, Stopband attenuation: 10.0 dB
      -0.043183226f, -0.046636667f, -0.049576525f, -0.051936015f, 
      -0.053661242f, -0.054712527f, 0.82598513f, -0.054712527f, 
      -0.053661242f, -0.051936015f, -0.049576525f, -0.046636667f, 
      -0.043183226f, 0.0f
    };
    memmove(z+1, z, 13*sizeof(REAL));
    z[0] = in;
    REAL sum0 = 0.0, sum1 = 0.0;
    int j;

    for (j = 0; j < 14; j+= 2) {
      // optimize: partial loop unrolling
      sum0 += a[j] * z[j];
      sum1 += a[j+1] * z[j+1];
    }
    return sum0+sum1;
  }
};


/* Recursive single pole IIR Infinite Impulse response filter
 * Coefficients calculated with
 * http://www.dsptutor.freeuk.com/IIRFilterDesign/IIRFiltDes102.html
 */
class IIR1 {
  REAL x, y;

public:
   IIR1() { memset(this, 0, sizeof(IIR1)); };
  REAL highpass(REAL in) {
    // Chebyshev IIR filter, Filter type: HP
    // Passband: 3700 - 4000.0 Hz
    // Passband ripple: 1.5 dB, Order: 1
    const REAL a0 = 0.105831884f;
    const REAL a1 = -0.105831884;
    const REAL b1 = 0.78833646f;
    REAL out = a0 * in + a1 * x + b1 * y;
    x = in;
    y = out;
    return out;
  }
};

/* Recursive two pole IIR Infinite Impulse Response filter
 * Coefficients calculated with
 * http://www.dsptutor.freeuk.com/IIRFilterDesign/IIRFiltDes102.html
 */
class IIR2 {
  REAL x[2], y[2];

public:
   IIR2() { memset(this, 0, sizeof(IIR2)); };
  REAL highpass(REAL in) {
    // Butterworth IIR filter, Filter type: HP
    // Passband: 2000 - 4000.0 Hz, Order: 2
    const REAL a[] = { 0.29289323f, -0.58578646f, 0.29289323f };
    const REAL b[] = { 1.3007072E-16f, 0.17157288f };
    REAL out =
      a[0] * in +
      a[1] * x[0] +
      a[2] * x[1] -
      b[0] * y[0] -
      b[1] * y[1];

    x[1] = x[0];
    x[0] = in;
    y[1] = y[0];
    y[0] = out;
    return out;
  }
};

// Extention in taps to reduce mem copies
#define NLMS_EXT  (10*8)  

// block size in taps to optimize DTD calculation 
#define DTD_LEN   16       


class AEC {
  // Time domain Filters
  IIR_HP hp00, hp1;             // DC-level remove Highpass)
  FIR_HP13 hp0;                 // 300Hz cut-off Highpass
  IIR1 Fx, Fe;                  // pre-whitening Highpass for x, e

  // Geigel DTD (Double Talk Detector)
  REAL max_max_x;               // max(|x[0]|, .. |x[L-1]|)
  int hangover;
  // optimize: less calculations for max()
  REAL max_x[NLMS_LEN / DTD_LEN];  
  int dtdCnt;
  int dtdNdx;

  // NLMS-pw
  REAL x[NLMS_LEN + NLMS_EXT];  // tap delayed loudspeaker signal
  REAL xf[NLMS_LEN + NLMS_EXT]; // pre-whitening tap delayed signal
  REAL w[NLMS_LEN];             // tap weights
  int j;                        // optimize: less memory copies
  int lastupdate;               // optimize: iterative dotp(x,x)
  double dotp_xf_xf;            // double to avoid loss of precision
  double Min_dotp_xf_xf;
  REAL s0avg;

public:
   AEC();

/* Geigel Double-Talk Detector
 *
 * in d: microphone sample (PCM as REALing point value)
 * in x: loudspeaker sample (PCM as REALing point value)
 * return: 0 for no talking, 1 for talking
 */
  int dtd(REAL d, REAL x);

/* Normalized Least Mean Square Algorithm pre-whitening (NLMS-pw)
 * The LMS algorithm was developed by Bernard Widrow
 * book: Widrow/Stearns, Adaptive Signal Processing, Prentice-Hall, 1985
 *
 * in mic: microphone sample (PCM as REALing point value)
 * in spk: loudspeaker sample (PCM as REALing point value)
 * in update: 0 for convolve only, 1 for convolve and update 
 * return: echo cancelled microphone sample
 */
  REAL nlms_pw(REAL mic, REAL spk, int update);

/* Acoustic Echo Cancellation and Suppression of one sample
 * in   d:  microphone signal with echo
 * in   x:  loudspeaker signal
 * return:  echo cancelled microphone signal
 */
  int doAEC(int d, int x);

  float getambient() {
    return s0avg;
  };
  void setambient(float Min_xf) {
    dotp_xf_xf = Min_dotp_xf_xf = NLMS_LEN * Min_xf * Min_xf;
  };
};

#define _AEC_H
#endif
 
