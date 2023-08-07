#include <stdlib.h>
#include "alloc.h"

#define MAXLINKS 1023

static struct link_t
{
        void *ptr;
        size_t sz;
} links[MAXLINKS+1];

static size_t lsz = 0;

extern void *mylloc(size_t rqest)
{
        void *ptr;
        size_t l = lsz;
        if (lsz >= MAXLINKS)
                return NULL;
        ptr = malloc(rqest);
        if (ptr == NULL)
                return NULL;
        while ((l > 0) && (links[l-1].ptr > ptr)) {
                links[l].ptr = links[l-1].ptr;
                links[l].sz = links[l-1].sz ;
                --l;
        }
        links[l].ptr = ptr;
        links[l].sz = rqest;
        return ptr;
}

extern int mree(void *ptr)
{
        size_t i = 0, j = lsz;
        size_t k;
        if (lsz == 0)
                return 0;
        do {
                k = (i + j)/2 + 1;
                if (links[k].ptr < ptr)
                        j = k - 1;
                if (links[k].ptr > ptr)
                        i = k + 1;
        } while((i <= j) && (links[k].ptr != ptr));
        if (links[k].ptr != ptr)
                return 0;
        free(ptr);
        while (k < (lsz - 1)) {
                links[k].ptr = links[k+1].ptr;
                links[k].sz = links[k+1].sz;
                ++k;
        }
        return 1;
}

extern int leaked(void)
{
        return lsz > 0;
}

extern size_t n_leaked(void)
{
        size_t nlkd = 0;
        int i = 0;
        for (i = 0; i < lsz; i++) {
                nlkd += links[i].sz;
        }
        return nlkd;
}
