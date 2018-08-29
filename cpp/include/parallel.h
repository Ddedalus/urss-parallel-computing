#ifndef __PARALLEL_H__
#define __PARALLEL_H__

#include "algorithms.h"

uint64_t sspParaBitset(const vecGraph &g, int maxThreads);
uint64_t sspParaBFS(const vecGraph &g, int maxThreads);


#endif