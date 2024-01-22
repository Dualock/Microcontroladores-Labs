
#ifndef DISPLAY7SEG_H
#define DISPLAY7SEG_H

#define LOAD_SIZE_BIT1 PB3
#define LOAD_SIZE_BIT2 PB4
#define LOW_WATER 5
#define MED_WATER 6
#define HIGH_WATER 7
#define LOW_WASH 7
#define MED_WASH 5
#define HIGH_WASH 8
#define LOW_RINSE 5
#define MED_RINSE 6
#define HIGH_RINSE 9
#define LOW_SPIN 4
#define MED_SPIN 6
#define HIGH_SPIN 7

/// *********************** Function prototypes *************************
void loadSize(unsigned char, unsigned char*);
#endif
