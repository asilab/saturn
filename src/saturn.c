#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

#include "mem.h"
#include "defs.h"
#include "msg.h"
#include "buffer.h"
#include "alphabet.h"
#include "sequence.h"
#include "levels.h"
#include "common.h"
#include "pmodels.h"
#include "tolerant.h"
#include "context.h"
#include "turing.h"
#include "bitio.h"
#include "arith.h"
#include "arith_aux.h"

ALPHABET *AL;
SEQUENCE *SEQ;
CModel   **RMS;

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - C O M P R E S S O R - - - - - - - - - - - - - -

void CompressTarget(Threads T){
  double      bits = 0;
  uint64_t    nBase = 0, r = 0, nSymbol, initNSymbol, idx;
  uint32_t    n, k, idxPos, totModels, cModel;
  CBUF        *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);
  uint8_t     sym, *pos;
  PModel      **pModel, *MX;
  CModel      **Shadow; // SHADOWS FOR SUPPORTING MODELS WITH THREADING
  FloatPModel *PT;
  CMWeight    *CMW;

  totModels = P->nModels; // EXTRA MODELS DERIVED FROM EDITS
  for(n = 0 ; n < P->nModels ; ++n)
    if(T.model[n].edits != 0)
      totModels += 1;

  Shadow      = (CModel **) Calloc(P->nModels, sizeof(CModel *));
  for(n = 0 ; n < P->nModels ; ++n)
    Shadow[n] = CreateShadowModel(RMS[n]);
  pModel      = (PModel **) Calloc(totModels, sizeof(PModel *));
  for(n = 0 ; n < totModels ; ++n)
    pModel[n] = CreatePModel(AL->cardinality);
  MX          = CreatePModel(AL->cardinality);
  PT          = CreateFloatPModel(AL->cardinality);
  CMW         = CreateWeightModel(totModels);

  char name_o[4096];
  sprintf(name_o, "%s.%u", "sat-inv-tmp", T.id);
  FILE *Writer = Fopen(name_o, "w");

  for(idx = 0 ; idx < SEQ->size ; ++idx){
    symBuf->buf[symBuf->idx] = sym = AL->revMap[SEQ->seq[idx]];
    memset((void *)PT->freqs, 0, AL->cardinality * sizeof(double));
    n = 0;
    pos = &symBuf->buf[symBuf->idx-1];
    for(cModel = 0 ; cModel < P->nModels ; ++cModel){
      CModel *CM = Shadow[cModel];
      GetPModelIdx(pos, CM);
      ComputePModel(RMS[cModel], pModel[n], CM->pModelIdx, CM->alphaDen);
      ComputeWeightedFreqs(CMW->weight[n], pModel[n], PT, CM->nSym);

      if(CM->edits != 0){
        ++n;
        CM->TM->seq->buf[CM->TM->seq->idx] = sym;
        CM->TM->idx = GetPModelIdxCorr(CM->TM->seq->buf+
        CM->TM->seq->idx-1, CM, CM->TM->idx);
        ComputePModel(RMS[cModel], pModel[n], CM->TM->idx, CM->TM->den);
        ComputeWeightedFreqs(CMW->weight[n], pModel[n], PT, CM->nSym);
        }
      ++n;
      }

    ComputeMXProbs(PT, MX, AL->cardinality);
    bits += PModelSymbolLog(MX, sym);
    CalcDecayment(CMW, pModel, sym, P->gamma);
    RenormalizeWeights(CMW);
    CorrectXModels(Shadow, pModel, sym);
    UpdateCBuffer(symBuf);
    }

  DeleteWeightModel(CMW);
  for(n = 0 ; n < totModels ; ++n)
    RemovePModel(pModel[n]);
  Free(pModel);
  RemovePModel(MX);
  RemoveFPModel(PT);
  for(n = 0 ; n < P->nModels ; ++n)
    FreeShadow(Shadow[n]);
  Free(Shadow);
  RemoveCBuffer(symBuf);
  fclose(Writer);



/*
  uint32_t    n, k, x, cModel, totModels, idx;
  uint64_t    i, compressed_bytes = 0;
  uint8_t     sym, *pos;
  double      se = 0;
  PModel      **pModel, *MX;
  FloatPModel *PT;
  CMWeight    *WM;
  CBUF        *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);

  fprintf(stderr, "Analyzing data and creating models ...\n");

  // EXTRA MODELS DERIVED FROM EDITS
  totModels = P->nModels;
  for(n = 0 ; n < P->nModels ; ++n) 
    if(P->model[n].edits != 0){
      totModels++;
      }

  fprintf(stderr, "Using %u probabilistic models\n", totModels);

  pModel        = (PModel  **) Calloc(totModels, sizeof(PModel *));
  for(n = 0 ; n < totModels ; ++n)
    pModel[n]   = CreatePModel(AL->cardinality);
  MX            = CreatePModel(AL->cardinality);
  PT            = CreateFloatPModel(AL->cardinality);
  WM            = CreateWeightModel(totModels);

  for(n = 0 ; n < P->nModels ; ++n)
    if(P->model[n].type == TARGET){
      cModels[n] = CreateCModel(P->model[n].ctx, P->model[n].den, TARGET, 
      P->model[n].edits, P->model[n].eDen, AL->cardinality, P->model[n].gamma,
      P->model[n].eGamma);
      }

  if(P->verbose){
    fprintf(stderr, "Done!\n");
    fprintf(stderr, "Compressing target sequence %d [%"PRIu64" symbols] ...\n", 
    id + 1, SEQ->size);
    }

  // GIVE SPECIFIC GAMMA:
  int pIdx = 0;
  for(n = 0 ; n < P->nModels ; ++n){
    WM->gamma[pIdx++] = cModels[n]->gamma;
    if(P->model[n].edits != 0){
      WM->gamma[pIdx++] = cModels[n]->eGamma;
      }
    }

  i = 0;
  for(idx = 0 ; idx < SEQ->size ; ++idx){

    CalcProgress(SEQ->size, ++i);
      
    symBuf->buf[symBuf->idx] = sym = AL->revMap[SEQ->seq[idx]];
    memset((void *)PT->freqs, 0, AL->cardinality * sizeof(double));

    n = 0;
    pos = &symBuf->buf[symBuf->idx-1];
    for(cModel = 0 ; cModel < P->nModels ; ++cModel){
      CModel *CM = cModels[cModel];
      GetPModelIdx(pos, CM);
      ComputePModel(CM, pModel[n], CM->pModelIdx, CM->alphaDen);
      ComputeWeightedFreqs(WM->weight[n], pModel[n], PT, CM->nSym);
      if(CM->edits != 0){
        ++n;
        CM->TM->seq->buf[CM->TM->seq->idx] = sym;
        CM->TM->idx = GetPModelIdxCorr(CM->TM->seq->buf+
        CM->TM->seq->idx-1, CM, CM->TM->idx);
        ComputePModel(CM, pModel[n], CM->TM->idx, CM->TM->den);
        ComputeWeightedFreqs(WM->weight[n], pModel[n], PT, CM->nSym);
        }
      ++n;
      }

    ComputeMXProbs(PT, MX, AL->cardinality);

    compressed_bytes += (PModelSymbolNats(MX, sym) / M_LN2);

    CalcDecayment(WM, pModel, sym);

    for(n = 0 ; n < P->nModels ; ++n)
      if(cModels[n]->ref == TARGET)
        UpdateCModelCounter(cModels[n], sym, cModels[n]->pModelIdx);

    RenormalizeWeights(WM);

    n = 0;
    for(cModel = 0 ; cModel < P->nModels ; ++cModel){
      if(cModels[cModel]->edits != 0)
        UpdateTolerantModel(cModels[cModel]->TM, pModel[++n], sym);
      ++n;
      }

    UpdateCBuffer(symBuf);
    }

  Free(MX);
  for(n = 0 ; n < P->nModels ; ++n)
    if(P->model[n].type == REFERENCE)
      ResetCModelIdx(cModels[n]);
    else
      RemoveCModel(cModels[n]);
  for(n = 0 ; n < totModels ; ++n){
    RemovePModel(pModel[n]);
    }
  Free(pModel);

  Free(PT);
  RemoveCBuffer(symBuf);
  RemoveWeightModel(WM);

  if(P->verbose == 1)
    fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID 

  fprintf(stderr, "Total bytes: %"PRIu64"\n", compressed_bytes);
*/
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - F   T H R E A D I N G - - - - - - - - - - - - - - -

void *CompressThread(void *Thr){
  Threads *T = (Threads *) Thr;
  CompressTarget(T[0]);
  pthread_exit(NULL);
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - R E F E R E N C E - - - - - - - - - - - - -

void LoadReference(Parameters *P){
  uint32_t  n, k, idx;
  uint64_t  i = 0;
  int       sym;
  CBUF      *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);
 
  for(idx = 0 ; idx < SEQ->size ; ++idx){
    symBuf->buf[symBuf->idx] = sym = AL->revMap[SEQ->seq[idx]];
    for(n = 0 ; n < P->nModels ; ++n){
      GetPModelIdx(symBuf->buf+symBuf->idx-1, RMS[n]);
      UpdateCModelCounter(RMS[n], sym, RMS[n]->pModelIdx);
      }
    UpdateCBuffer(symBuf);
    CalcProgress(SEQ->size, ++i);
    }
 
  for(n = 0 ; n < P->nModels ; ++n)
    ResetCModelIdx(RMS[n]);

  RemoveCBuffer(symBuf);
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - A C T I O N - - - - - - - - - - - - - - -

void Action(Parameters *P, Threads *T){
  pthread_t t[P->nThreads];
  uint32_t n;

  fprintf(stderr, "Loading alphabet ...\n");
  AL = CreateAlphabet();
  LoadAlphabet(AL, stdin);
  PrintAlphabet(AL);
  fprintf(stderr, "Done!\n");

  fprintf(stderr, "Loading sequence to memory ...\n");
  SEQ = CreateSequence();
  LoadSequence(SEQ, stdin);
  fprintf(stderr, "Done!\n");

  fprintf(stderr, "Creating reference models ...\n");
  RMS = (CModel **) Malloc(P->nModels * sizeof(CModel *));
  for(n = 0 ; n < P->nModels ; ++n)
    RMS[n] = CreateCModel(T[0].model[n].ctx, T[0].model[n].den, REFERENCE,
    T[0].model[n].edits, T[0].model[n].eDen, AL->cardinality, 0.9, 0.9);
  fprintf(stderr, "Done!\n");

  fprintf(stderr, "Loading reference models ...\n");
  LoadReference(P);
  fprintf(stderr, "Done!\n");

  fprintf(stderr, "Running machines ... ");
  for(n = 0 ; n < P->nThreads ; ++n)
    pthread_create(&(t[n+1]), NULL, CompressThread, (void *) &(T[n]));
  for(n = 0 ; n < P->nThreads ; ++n) // DO NOT JOIN FORS!
    pthread_join(t[n+1], NULL);
  fprintf(stderr, "Done!\n");

/*
  fprintf(stderr, "  [+] Joinning streams ............. ");
  FILE *OUT = Fopen(P->output, "w");
  FILE *IN  = Fopen(P->base,   "r");
  FILE **TMP = (FILE **) Calloc(P->nThreads, sizeof(FILE *));
  for(n = 0 ; n < P->nThreads ; ++n){
    char name_o[MAX_NAME_OUT];
    sprintf(name_o, "%s.%u", P->output, n);
    TMP[n] = Fopen(name_o, "r");
    }
  Read *Read = CreateRead(10000, 40000);
  n = 0;
  while((Read = GetRead(IN, Read)) != NULL){
    if(fgetc(TMP[n++ % P->nThreads]) == '1')
      PutRead(Read, OUT);
    }
  for(n = 0 ; n < P->nThreads ; ++n){
    fclose(TMP[n]);
    char name_o[MAX_NAME_OUT];
    sprintf(name_o, "%s.%u", P->output, n);
    Fdelete(name_o);
    }
  fclose(IN);
  fclose(OUT);
  fprintf(stderr, "Done!\n");
*/
  }


  
//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[]){
  char        **p = *&argv, **xargv, *xpl = NULL;
  int32_t     xargc = 0, cardinality = 1;
  uint32_t    n, k, nModels, ref;
  uint64_t    totalBytes, headerBytes, totalSize;
  clock_t     stop = 0, start = clock();
  double      se_average;
  Threads     *T;
  //Parameters  *P;

  P = (Parameters *) Malloc(1 * sizeof(Parameters));
  if((P->help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2){
    PrintMenu();
    return EXIT_SUCCESS;
    }

  if(ArgsState(DEF_VERSION, p, argc, "-V")){
    PrintVersion();
    return EXIT_SUCCESS;
    }

  if(ArgsState(0, p, argc, "-s")){
    PrintLevels(); 
    return EXIT_SUCCESS;
    }

  P->verbose   = ArgsState  (DEFAULT_VERBOSE, p, argc, "-v" );
  P->force     = ArgsState  (DEFAULT_FORCE,   p, argc, "-f" );
  P->estim     = ArgsState  (0,               p, argc, "-e" );
  P->level     = ArgsNum    (0,               p, argc, "-l", MIN_LEVEL, MAX_LEVEL);
  P->nThreads  = ArgsNum    (DEFAULT_THREADS, p, argc, "-n", MIN_THREADS,
  MAX_THREADS);
  P->threshold = fabs(ArgsDouble (0.6,   p, argc, "-t"));

  P->nModels = 0;
  for(n = 1 ; n < argc ; ++n)
    if(strcmp(argv[n], "-em") == 0 || strcmp(argv[n], "-em") == 0)
      P->nModels += 1;

  if(P->nModels == 0 && P->level == 0)
    P->level = DEFAULT_LEVEL;
  
  if(P->level != 0){
    xpl = GetLevels(P->level);
    xargc = StrToArgv(xpl, &xargv);
    for(n = 1 ; n < xargc ; ++n)
      if(strcmp(xargv[n], "-em") == 0)
        P->nModels += 1;
    }

  if(P->nModels == 0){
    fprintf(stderr, "Error: at least you need to use a context model!\n");
    return 1;
    }

  // READ MODEL PARAMETERS FROM XARGS & ARGS
  T = (Threads *) Calloc(P->nThreads, sizeof(Threads));
  for(ref = 0 ; ref < P->nThreads ; ++ref){
    T[ref].model = (ModelPar *) Calloc(P->nModels, sizeof(ModelPar));
    T[ref].id    = ref;
    k = 0;
    for(n = 1 ; n < argc ; ++n)
      if(strcmp(argv[n], "-em") == 0)
        T[ref].model[k++] = ArgsUniqModel(argv[n+1], 0);
    if(P->level != 0){
      for(n = 1 ; n < xargc ; ++n)
        if(strcmp(xargv[n], "-em") == 0)
          T[ref].model[k++] = ArgsUniqModel(xargv[n+1], 0);
      }
    }

  if(P->verbose) 
    PrintArgs(P, T[0]);

  Action(P, T);

  for(ref = 0 ; ref < P->nThreads ; ++ref)
    Free(T[ref].model);
  Free(T);

  stop = clock();
  fprintf(stdout, "Spent %g sec.\n", ((double)(stop-start))/CLOCKS_PER_SEC);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
