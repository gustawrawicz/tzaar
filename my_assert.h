#ifndef _MY_ASSERT_H_
#define _MY_ASSERT_H_

#include <cassert>

#define MY_ASSERT(x) /*assert(x)*/ if(!(x)){int * g; g=NULL; g[1]=9;}

#endif