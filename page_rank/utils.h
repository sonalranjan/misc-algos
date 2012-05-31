#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>

extern int debug;

#define ASSERT(expr)  assert(expr)
#define DEBUG(expr) { \
    if (debug) { std::cout << "DEBUG: " << expr << std::endl; } \
}
#define INFO(expr) { \
    std::cout << "INFO: " << expr << std::endl; \
}


#endif

