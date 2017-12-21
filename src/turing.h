#ifndef TURING_H_INCLUDED
#define TURING_H_INCLUDED

#include "defs.h"

typedef struct{
  int8_t      *tape;
  int32_t     n_tape;
  int32_t     max_n_tape;
  int32_t     **rules;
  int32_t     n_states;
  int32_t     position;
  int32_t     n_alphabet;
  int32_t     *alphabet;
  int32_t     time;
  }
T_MACHINE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

T_MACHINE       *Create_TM    (uint32_t, uint32_t, uint8_t, uint32_t);
void            Update_TM     (T_MACHINE *);
void            Remove_TM     (T_MACHINE *);
void            Reset_TM_Tape (T_MACHINE *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
