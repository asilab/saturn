#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include "defs.h"


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct{
  uint64_t size;
  uint8_t  *seq;
  }
SEQUENCE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

SEQUENCE   *CreateSequence       (void);
void       LoadSequence          (SEQUENCE *, FILE *);
void       RemoveSequence        (SEQUENCE *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
