#include "hash.h"

#define FNV_PRIME       16777619ul
#define FNV_OFFSET      2166136261ul

static unsigned long fnv1a_32(const char *key)
{
        unsigned long hash = FNV_OFFSET;
        while (*key) {
                hash ^= (unsigned long)(*key);
                hash *= FNV_PRIME;
                ++key;
        }
        return hash;
}

extern unsigned long hash(const char *key)
{
        return fnv1a_32(key);
}
