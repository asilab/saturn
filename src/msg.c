#include "msg.h"
#include <stdio.h>
#include <stdlib.h>

void PrintMenu(void){
  fprintf(stderr,
  "                                                                         \n"
  "Usage: SATURN [OPTION]... < [SEQUENCE] > [OUTPUT]                        \n"
  "                                                                         \n"
  "SATURN: a tool to find SmAll TURiNg machines.                            \n"
  "                                                                         \n"
  "Non-mandatory arguments:                                                 \n"
  "                                                                         \n"
  "  -h                       give this help,                               \n"
  "  -F                       force mode (overwrites top file),             \n"
  "  -V                       display version number,                       \n"
  "  -v                       verbose mode (more information),              \n"
  "  -t <double>              similarity threshold [0.0;1.0],               \n"
  "  -m <integer>             maximum time allowed without halting,         \n"
  "  -n <integer>             number of threads (default: %u).              \n"
  "                                                                         \n"
  "Mandatory arguments:                                                     \n"
  "                                                                         \n"
  "  < [std  in]              sequence,                                     \n"
  "  > [std out]              output results.                               \n"
  "                                                                         \n"
  "Report issues to <{pratas}@ua.pt>.                                       \n"
  "                                                                         \n",
  DEF_THREADS);
  }

void PrintVersion(void){
  fprintf(stderr,
  "                                                                         \n"
  "                          ===================                            \n"
  "                          |   SATURN  %u.%u   |                          \n"
  "                          ===================                            \n"
  "                                                                         \n"
  "                 A tool the find SmAll TURiNg machines.                  \n"
  "                                                                         \n"
  "              Copyright (C) 2017-2018 University of Aveiro.              \n"
  "                                                                         \n"
  "                  This is a Free software, under GPLv3.                  \n"
  "                                                                         \n"
  "You may redistribute copies of it under the terms of the GNU - General   \n"
  "Public License v3 <http://www.gnu.org/licenses/gpl.html>. There is NOT   \n"
  "ANY WARRANTY, to the extent permitted by law. Developed and Written by   \n"
  "Diogo Pratas.\n\n", VERSION, RELEASE);
  }

