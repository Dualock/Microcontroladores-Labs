
#ifndef DISPLAY7SEG_H
#define DISPLAY7SEG_H

#define LOAD_SIZE_BIT1 PB3
#define LOAD_SIZE_BIT2 PB4
#define LOW_WATER 1
#define MED_WATER 2
#define HIGH_WATER 3
#define LOW_WASH 3
#define MED_WASH 5
#define HIGH_WASH 7
#define LOW_RINSE 2
#define MED_RINSE 4
#define HIGH_RINSE 5
#define LOW_SPIN 3
#define MED_SPIN 5
#define HIGH_SPIN 6

/// *********************** Function prototypes *************************
void loadSize(unsigned char, unsigned char*);
#endif
