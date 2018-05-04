#include <stdio.h>
#include <stdlib.h>
#include "turing.h"
#include "mem.h"

/*
  int8_t      *tape;
  int32_t     n_tape;
  int32_t     **rules;
  int32_t     n_states;
  int32_t     position;
  int32_t     n_alphabet;
  int32_t     *alphabet;
  int32_t     time;
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

T_MACHINE *Create_TM(uint32_t a, uint32_t b, uint8_t c, uint32_t d){


  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Update_TM(T_MACHINE *TM){

  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Remove_TM(T_MACHINE *TM){
  int32_t x;
  for(x = 0 ; x < TM->n_alphabet ; ++x)
    Free(TM->rules);
  Free(TM->rules); 
  Free(TM->alphabet); 
  Free(TM->tape); 
  Free(TM); 
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Reset_TM_Tape(T_MACHINE *TM){
  Free(TM->tape);
  TM->n_tape = DEFAULT_TAPE_SIZE;
  TM->tape   = (uint8_t *) Calloc(TM->n_tape, sizeof(uint8_t));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
