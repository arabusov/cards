#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "table.h"
#include "hash.h"
#include "error.h"
#include "alloc.h"

#define MAXRECS 1023
#define MAXCHRS 30

static struct table_t
{
        unsigned long hash;
        size_t sz;
        char *w;
        unsigned long link;
} t1[MAXRECS+1], t2[MAXRECS+2];

static size_t tsz = 0;

static void alloc_record(struct table_t *r, const char *s)
{
        r->sz = strlen(s);
        if (r->sz >= MAXCHRS)
                error("Maximum number of chars per a word is exceeded");
        r->w = mylloc(r->sz + 1);
        if (r->w == NULL)
                error("Not enough memory or RAM to allocate a record");
        strcpy(r->w, s);
        r->hash = hash(r->w);
}

static void copy_recs(struct table_t *dst, const struct table_t *src)
{
        dst->w = src->w;
        dst->sz = src->sz;
        dst->hash = src->hash;
        dst->link = src->link;
}

static void
insert_record(struct table_t *t, size_t sz, struct table_t *r, unsigned long l)
{
        size_t ssz = sz;
        while ((t[ssz-1].hash > r->hash) && (ssz > 0)) {
                copy_recs(t+ssz, t+ssz-1);
                ssz --;
        }
        if (ssz == 0) {
                copy_recs(t, r);
                t->link = l;
                return;
        }
        copy_recs(t+ssz-1, r);
        t[ssz-1].link = l;
}

extern void add_word(const char *s1, const char *s2)
{
        struct table_t r1, r2;
        if (tsz >= MAXRECS)
                error("Maximum table size is reached");
        alloc_record(&r1, s1);
        alloc_record(&r2, s2);
        insert_record(t1, tsz, &r1, r2.hash);
        insert_record(t2, tsz, &r2, r1.hash);
        ++tsz;
}

static void reset_table(struct table_t *t)
{
        int i;
        for (i = 0; i < tsz; i++) {
                if ((t+i)->w != NULL)
                        mree((t+i)->w);
        }
}

extern void reset_tables(void)
{
        reset_table(t1);
        reset_table(t2);
        tsz = 0;
}

static int find_hash(unsigned long hash, const struct table_t *t, size_t sz)
{
        int i = 0, j = sz-1, k;
        do {
                k = (i + j) / 2 + 1;
                if (t[k].hash < hash)
                        j = k - 1;
                if (t[k].hash > hash)
                        i = k + 1;
        } while ((i <= j) && (t[k].hash != hash));
        if (t[k].hash == hash)
                return k;
        return -1;
}

static void mkdummy(struct table_t *t, size_t *sz)
{
        int i;
        struct table_t dummy;
        *sz = 0;
        for (i = 9; i >= 0; i--) {
                dummy.hash = i;
                insert_record(t, (*sz)++, &dummy, 0);
        }
}

static int test_insert(void)
{
        struct table_t t[10];
        int i;
        size_t sz;
        mkdummy(t, &sz);
        for (i = 1; i < 10; i++) {
                if (t[i-1].hash > t[i].hash) {
                        fprintf(stderr, "Wrong hash order at %d pos\n", i);
                        return 0;
                }
        }
        return 1;
}

static int test_find_hash(void)
{
        struct table_t t[10];
        size_t sz;
        mkdummy(t, &sz);
        if (find_hash(5, t, sz) == -1) {
                fprintf(stderr, "Failed to find an existing element\n");
                return 0;
        }
        return 1;
}

static int test_hash_dir(const struct table_t *tl, const struct table_t *tr)
{
        int i;
        for (i = 0; i < tsz; i++) {
                unsigned long h;
                if ((h = find_hash(tl[i].hash, tr, tsz)) == -1)
                        return 0;
        }
        return 1;
}

static int test_hashes(void)
{
        char dummy[][2][MAXCHRS+2] = {
                { "abc", "cde" },
                { "kde", "" },
                { "&&&ecoG", "eg<4" }
        };
        size_t nex = sizeof(dummy)/2/(MAXCHRS+2);
        int i;
        for (i = 0; i < nex; i++) {
                add_word(dummy[i][0], dummy[i][1]);
        }
        if (nex != tsz) {
                fprintf(stderr, "Test size %lu does not match\
with the table size %lu", nex, tsz);
                reset_tables();
                return 0;
        }
        if (test_hash_dir(t1, t2) == 0) {
                fprintf(stderr, "Left-to-right links are broken\n");
                reset_tables();
                return 0;
        }
        if (test_hash_dir(t2, t1) == 0) {
                fprintf(stderr, "Right-to-left links are broken\n");
                reset_tables();
                return 0;
        }
        reset_tables();
        if (leaked())
                fprintf(stderr, "Warning, %lu bytes leaked\n", n_leaked());
        return 1;
}

extern int run_table_tests(void)
{
        size_t n_tests = 0, n_succ = 0;
        n_succ += test_insert(); n_tests++;
        n_succ += test_find_hash(); n_tests++;
        n_succ += test_hashes(); n_tests++;
        printf("Table tests: ");
        if (n_tests == n_succ) {
                printf("Ok [%lu/%lu]\n", n_succ, n_tests);
                return 1;
        }
        printf("[failed %lu/%lu]\n", n_tests-n_succ, n_tests);
        return 0;
}

