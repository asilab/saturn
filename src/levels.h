#ifndef LEVELS_H_INCLUDED
#define LEVELS_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
#define LEVEL_1 " 1: -em 1:1:0.9/0:0:0 "
#define LEVEL_2 " 2: -em 3:1:0.9/0:0:0 "
#define LEVEL_3 " 3: -em 5:20:0.9/1:1:0.9 "
#define LEVEL_4 " 4: -em 1:1:0.9/0:0:0 -em 5:20:0.9/1:1:0.9 "
#define LEVEL_5 " 5: -em 1:1:0.9/0:0:0 -em 3:1:0.9/0:0:0 -em 5:20:0.9/1:1:0.9 "

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char    *GetLevels  (uint8_t);
void    PrintLevels (void);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

