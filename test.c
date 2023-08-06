#include <stdio.h>
#include "hash.h"
#include "table.h"
#include "error.h"

int main(void)
{
        if (run_table_tests())
                return 0;
        return ERR_CODE;
}
