#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"
#include "mem.h"
#include "common.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Print Shannon Entropy
//
double PrintSE(ALPHABET *A){
  int x;
  double se = 0;
  for(x = 0 ; x < A->cardinality ; ++x){
    double prob = (double) A->counts[(int) A->toChars[x]] / A->length;
    se += (prob * Log(A->cardinality, prob));
    }
  return -se;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CREATE ALPHABET
//
ALPHABET *CreateAlphabet(){
  ALPHABET *A        = (ALPHABET *) Calloc(1,                 sizeof(ALPHABET));
  A->numeric         = (uint8_t  *) Calloc(ALPHABET_MAX_SIZE, sizeof(uint8_t));
  A->toChars         = (uint8_t  *) Calloc(ALPHABET_MAX_SIZE, sizeof(uint8_t));
  A->revMap          = (uint8_t  *) Calloc(ALPHABET_MAX_SIZE, sizeof(uint8_t));
  A->alphabet        = (uint8_t  *) Calloc(ALPHABET_MAX_SIZE, sizeof(uint8_t));
  A->mask            = (uint8_t  *) Calloc(ALPHABET_MAX_SIZE, sizeof(uint8_t));
  A->counts          = (uint64_t *) Calloc(ALPHABET_MAX_SIZE, sizeof(uint64_t));
  A->length          = 0;
  A->cardinality     = 0;
  return A;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RESET ALPHABET
//
void ResetAlphabet(ALPHABET *A){
  uint32_t x;
  A->cardinality = 0;
  for(x = 0 ; x < ALPHABET_MAX_SIZE ; x++){
    if(A->mask[x] == 1){
      A->toChars[A->cardinality] = x;
      A->revMap[x] = A->cardinality++;
      }
    else
      A->revMap[x] = INVALID_SYMBOL;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// LOAD ALPHABET
//
void LoadAlphabet(ALPHABET *A, FILE *F){
  uint64_t size = 0;
  uint32_t x;
  int32_t  k;
  uint8_t  *buffer;

  buffer = (uint8_t *) Calloc(BUFFER_SIZE, sizeof(uint8_t));
  while((k = fread(buffer, 1, BUFFER_SIZE, F)))
    for(x = 0 ; x < k ; ++x){
      A->mask[buffer[x]] = 1;
      A->counts[buffer[x]]++;
      ++size;
      }
  Free(buffer);
  A->length = size;

  ResetAlphabet(A);

  rewind(F);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PRINT ALPHABET
//
void PrintAlphabet(ALPHABET *A){
  int x;
  fprintf(stderr, "File size : %"PRIu64"\n", A->length);
  fprintf(stderr, "Alphabet size : %u\n", A->cardinality);
  if(A->cardinality == 256){
    fprintf(stderr, "ERROR: maximum cardinality allowed is 255 symbols!\n");
    exit(1);
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// REMOVE ALPHABET
//
void RemoveAlphabet(ALPHABET *A){
  Free(A->numeric);
  Free(A->toChars);
  Free(A->revMap);
  Free(A->alphabet);
  Free(A->mask);
  Free(A);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
