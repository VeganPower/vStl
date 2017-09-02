#include "config.h"
#include <cassert>

#if USE_ASSERT
#define V_ASSERT(pred) assert(pred)
#else
#define V_ASSERT(pred) 
#endif