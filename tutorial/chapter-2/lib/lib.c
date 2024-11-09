// --8<-- [start:snippet0]

#include "lib.h"

#ifndef CRUCIAL_DEFINE
#   error CRUCIAL_DEFINE not defined
#endif

const char *get_string()
{
    return "Hello from library";
}

// --8<-- [end:snippet0]
