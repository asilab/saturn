#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#include "mem.h"
#include "time.h"
#include "defs.h"
#include "param.h"
#include "msg.h"
#include "buffer.h"
#include "common.h"
#include "turing.h"


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - C O M P R E S S I O N - - - - - - - - - - - - - 

void RunTarget(Threads T){
  fprintf(stderr, "IN:%d\n", T.id);
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - F   T H R E A D I N G - - - - - - - - - - - - - - -

void *RunThread(void *Thr){
  Threads *T = (Threads *) Thr;
  RunTarget(T[0]);
  pthread_exit(NULL);
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - C O M P R E S S O R   M A I N - - - - - - - - - - - -

void RunAction(Threads *T, char *sequence){
  pthread_t t[P->nThreads];
  uint32_t n;
 
  fprintf(stderr, "  [+] Running ...... ");
  for(n = 0 ; n < P->nThreads ; ++n)
    pthread_create(&(t[n+1]), NULL, RunThread, (void *) &(T[n]));
  for(n = 0 ; n < P->nThreads ; ++n) // DO NOT JOIN FORS!
    pthread_join(t[n+1], NULL);
  fprintf(stderr, "Done!\n");

  return;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[]){
  char     **p = *&argv;
  uint32_t n, k, col, ref;
  double   gamma;
  Threads  *T;
  
  P = (Parameters *) Malloc(1 * sizeof(Parameters));
  if((P->help = ArgsState(DEF_HELP, p, argc, "-h")) == 1 || argc < 2){
    PrintMenu();
    return EXIT_SUCCESS;
    }

  if(ArgsState(DEF_VERSION, p, argc, "-V")){
    PrintVersion();
    return EXIT_SUCCESS;
    }

  P->verbose   = ArgsState(DEF_VERBOSE, p, argc, "-v");
  P->force     = ArgsState(DEF_FORCE,   p, argc, "-F");
  P->nThreads  = ArgsNum  (DEF_THREAD,  p, argc, "-n", MIN_THREAD, MAX_THREAD);
  P->threshold = fabs(ArgsDouble(0.9,   p, argc, "-t"));
  P->max_time  = ArgsNum  (DEF_MAXT,    p, argc, "-m", MIN_MAXT, MAX_MAXT);

  // READ MODEL PARAMETERS FROM ARGS
  T = (Threads *) Calloc(P->nThreads, sizeof(Threads));
  for(ref = 0 ; ref < P->nThreads ; ++ref){
    T[ref].id    = ref;
    }

  if(P->verbose){
    
    }

  fprintf(stderr, "==[ PROCESSING ]====================\n");
  TIME *Time = CreateClock(clock());
  RunAction(T, argv[argc-1]);
  fprintf(stderr, "Done!\n");
  StopTimeNDRM(Time, clock());
  fprintf(stderr, "\n");

  fprintf(stderr, "==[ RESULTS ]=======================\n");
  Free(T);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
