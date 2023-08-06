#include <stdio.h>
#include <stdlib.h>
#include "error.h"

extern void error(const char *s)
{
        fprintf(stderr, s);
        fprintf(stderr, "\n");
        exit(ERR_CODE);
}

