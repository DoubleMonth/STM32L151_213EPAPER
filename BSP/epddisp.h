#ifndef _EPDDISP__H_
#define _EPDDISP__H_
#include <stdlib.h>
#include <stdint.h>
#define COLORED      0
#define UNCOLORED    1
uint8_t epdDispFullUpdate(void);
void epdSleep(void);
uint8_t setPartialUpdate(void);
uint8_t setFullUpdate(void);
void updateMinute(void);
#endif
