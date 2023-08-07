#ifndef __ALLOC_H__
#define __ALLOC_H__
extern void *mylloc(size_t rqest);
extern int mree(void *ptr);
extern int leaked(void);
extern size_t n_leaked(void);
#endif /* __ALLOC_H__ */
