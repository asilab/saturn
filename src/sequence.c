#include <stdio.h>
#include "sequence.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CREATE SEQUENCE
//
SEQUENCE *CreateSequence(){
  SEQUENCE *S = (SEQUENCE *) Calloc(1, sizeof(SEQUENCE));
  S->size = 0;
  return S;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// LOAD SEQUENCE
//
void LoadSequence(SEQUENCE *S, FILE *F){
  uint64_t x, i = 0;
  int32_t  k;
  uint8_t  *buffer;

  S->size = NBytesInFile(F);
  S->seq  = (uint8_t *) Calloc(S->size + 1, sizeof(uint8_t));
  buffer  = (uint8_t *) Calloc(BUFFER_SIZE, sizeof(uint8_t));

  while((k = fread(buffer, 1, BUFFER_SIZE, F)))
    for(x = 0 ; x < k ; ++x)
      S->seq[i++] = buffer[x];
  S->seq[i] = '\0';

  Free(buffer);
  rewind(F);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// REMOVE SEQUENCE
//
void RemoveSequence(SEQUENCE *S){
  Free(S->seq);
  Free(S);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
