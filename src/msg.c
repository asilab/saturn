#include "msg.h"
#include <stdio.h>
#include <stdlib.h>

void ModelsExplanation(void){
  fprintf(stderr,
  "	-em <c>:<d>:<g>/<m>:<e>:<a>  model (-em 1:10:0.9/0:0:0),          \n"
  "	-em <c>:<d>:<g>/<m>:<e>:<a>  model (-em 5:90:0.9/1:50:0.8),       \n"
  "	...                                                               \n"
  "	                relative entropy model templates use <c> for      \n"
  "	                context-order size, <d> for alpha (1/<d>), <g>    \n"
  "	                for gamma (decayment forgetting factor) [0;1),    \n"
  "	                <m> to the maximum sets the allowed mutations,    \n"
  "	                on the context without being discarded (for       \n"
  "	                deep contexts), under the estimator <e>, using    \n"
  "	                <a> for gamma (decayment forgetting factor)       \n"
  "	                [0;1) (tolerant model),                           \n");
  } 

void PrintMenu(void){
  fprintf(stderr,
  "                                                                       \n"
  "NAME                                                                   \n"
  "	SATURN - Search for Approximate TURing machiNes.                  \n"
  "                                                                       \n"
  "SYNOPSIS                                                               \n"
  "	./SATURN [OPTION]... < [IN_FILE] > [OUT_FILE]                     \n"
  "                                                                       \n"
  "DESCRIPTION                                                            \n"
  "	This program searches for approximate Turing machines using       \n"
  "	as input symbolic sequences with a cardinality up to 255.         \n"
  "	For SATURN comments, suggestions and feedback please contact      \n"
  "	Diogo Pratas (pratas@ua.pt).                                      \n" 
  "                                                                       \n"
  "	-h              give this help,                                   \n"
  "	-V              display version number,                           \n"
  "                                                                       \n"
  "	-v              verbose mode (more information),                  \n"
  "	-f              force overwrite of output,                        \n"
  "	-n              number of threads (default: %u),                  \n"
  "                                                                       \n"
  "	-t <double>     similarity threshold [0.0;1.0],                   \n"
  "	-m <integer>    maximum iterations allowed without halting,       \n"
  "                                                                       \n"
  "	-s              show SATURN compression levels,                   \n"
  "	-l <integer>    level of compression [1;5] (lazy -em setup),      \n"
  "                                                                       \n",
  DEF_THREAD);

  ModelsExplanation();

  fprintf(stderr,
  "                                                                       \n"
  "	< [std  in]     input sequence from standard input,               \n"
  "	> [std out]     output results to standard output.                \n"
  "                                                                       \n"
  "EXAMPLE                                                                \n"
  "	./SATURN -v -l 3 < seq.txt                                        \n"
  "                                                                       \n"
  "COPYRIGHT                                                              \n"
  "	Copyright ©2017-2018, D. Pratas, IEETA, University of Aveiro.     \n"
  "	License GPLv3: GNU GPL v3 <http://gnu.org/licenses/gpl.html>.     \n"
  "	This is free software: you are free to change and redistribute    \n"
  "	it. There is NO WARRANTY, to the extent permitted by law.         \n"
  "                                                                       \n");
  }

void PrintVersion(void){
  fprintf(stderr,
  "                                                                       \n"
  "                          ===================                          \n"
  "                          |   SATURN  %u.%u   |                        \n"
  "                          ===================                          \n"
  "                                                                       \n"
  "            A tool to Search for Approximate TURing machiNes.          \n"
  "                                                                       \n"
  "       Copyright ©2017-2018, D. Pratas, IEETA, University of Aveiro.   \n"
  "                                                                       \n"
  "                  This is a Free software, under GPLv3.                \n"
  "                                                                       \n"
  "You may redistribute copies of it under the terms of the GNU - General \n"
  "Public License v3 <http://www.gnu.org/licenses/gpl.html>. There is NOT \n"
  "ANY WARRANTY, to the extent permitted by law. Developed and written by \n"
  "Diogo Pratas.\n\n", VERSION, RELEASE);
  }


