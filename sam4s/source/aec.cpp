/* aec.cpp
 *
 * Copyright (C) DFS Deutsche Flugsicherung (2004). All Rights Reserved.
 *
 * Acoustic Echo Cancellation NLMS-pw algorithm
 *
 * Version 1.3 filter created with www.dsptutor.freeuk.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "aec.h"


/* Vector Dot Product */
REAL dotp(REAL a[], REAL b[]) {
//  REAL sum0 = 0.0, sum1 = 0.0;
//  int j;
//  
//  for (j = 0; j < NLMS_LEN; j+= 2) {
//    // optimize: partial loop unrolling
//    sum0 += a[j] * b[j];
//    sum1 += a[j+1] * b[j+1];
//  }
//  return sum0+sum1;

	REAL c;
	arm_dot_prod_f32(a, b, NLMS_LEN, &c);
	return c;
}


AEC::AEC()
{
  max_max_x = 0.0f;
  hangover = 0;
  memset(max_x, 0, sizeof(max_x));
  dtdCnt = dtdNdx = 0;
  
  memset(x, 0, sizeof(x));
  memset(xf, 0, sizeof(xf));
  memset(w, 0, sizeof(w));
  j = NLMS_EXT;
  lastupdate = 0;
  s0avg = M80dB_PCM;
  setambient(Min_xf);
}

REAL AEC::nlms_pw(REAL mic, REAL spk, int update)
{
  REAL d = mic;      	        // desired signal
  x[j] = spk;
  xf[j] = Fx.highpass(spk);     // pre-whitening of x
  
  // calculate error value 
  // (mic signal - estimated mic signal from spk signal)

  REAL e = d - dotp(w, x + j);
//	REAL e;
//	arm_dot_prod_f32(w, x + j, NLMS_LEN, &e);
//	e = d - e;

  REAL ef = Fe.highpass(e);    // pre-whitening of e
  // optimize: iterative dotp(xf, xf)
  dotp_xf_xf += (xf[j]*xf[j] - xf[j+NLMS_LEN-1]*xf[j+NLMS_LEN-1]);
  if (update) {
    // calculate variable step size
    REAL mikro_ef = Stepsize * ef / dotp_xf_xf;

    // update tap weights (filter learning)
    int i;
    for (i = 0; i < NLMS_LEN; i += 2) {
      // optimize: partial loop unrolling
      w[i] += mikro_ef*xf[i+j];
      w[i+1] += mikro_ef*xf[i+j+1];
    }
  }

  if (--j < 0) {
    // optimize: decrease number of memory copies
    j = NLMS_EXT;
    memmove(x+j+1, x, (NLMS_LEN-1)*sizeof(REAL));    
    memmove(xf+j+1, xf, (NLMS_LEN-1)*sizeof(REAL));    
  }

  return e;
}


int AEC::dtd(REAL d, REAL x)
{
  // optimized implementation of max(|x[0]|, |x[1]|, .., |x[L-1]|):
  // calculate max of block (DTD_LEN values)
  x = fabsf(x);
  if (x > max_x[dtdNdx]) {
    max_x[dtdNdx] = x;
    if (x > max_max_x) {
      max_max_x = x;
    }
  }
  if (++dtdCnt >= DTD_LEN) {
    dtdCnt = 0;
    // calculate max of max
    max_max_x = 0.0f;
    for (int i = 0; i < NLMS_LEN/DTD_LEN; ++i) {
      if (max_x[i] > max_max_x) {
        max_max_x = max_x[i];
      }
    }
    // rotate Ndx
    if (++dtdNdx >= NLMS_LEN/DTD_LEN) dtdNdx = 0;
    max_x[dtdNdx] = 0.0f;
  }

  // The Geigel DTD algorithm with Hangover timer Thold
  if (fabsf(d) >= GeigelThreshold * max_max_x) {
    hangover = Thold;
  }
    
  if (hangover) --hangover;
  
  return (hangover > 0);
}


int AEC::doAEC(int d, int x) 
{
  REAL s0 = (REAL)d;
  REAL s1 = (REAL)x;
  
  // Mic Highpass Filter - to remove DC
//  s0 = hp00.highpass(s0);
  
  // Mic Highpass Filter - telephone users are used to 300Hz cut-off
//  s0 = hp0.highpass(s0);

  // ambient mic level estimation
//  s0avg += 1e-4f*(fabsf(s0) - s0avg);
  
  // Spk Highpass Filter - to remove DC
//  s1 = hp1.highpass(s1);

  // Double Talk Detector
//  int update = !dtd(s0, s1);
	int update = 1;

  // Acoustic Echo Cancellation
  s0 = nlms_pw(s0, s1, update);

  // Acoustic Echo Suppression
  if (update) {
    // Non Linear Processor (NLP): attenuate low volumes
    s0 *= NLPAttenuation;
  }
  
  // Saturation
  if (s0 > MAXPCM) {
    return (int)MAXPCM;
  } else if (s0 < -MAXPCM) {
    return (int)-MAXPCM;
  } else {
    return (int)(s0);
  }
}
 
