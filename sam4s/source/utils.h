#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>



/* --- GLOBAL VARIABLES ----------------------------------------------------- */

extern uint32_t volatile timer_ms;



/* --- EXPORTED FUNCTIONS --------------------------------------------------- */

void init(void);
void wait_ms(int interval_ms);



#endif