#ifndef __TABLE_H__
#define __TABLE_H__

enum dir_t { LR, RL };
extern void add_word(const char *s1, const char *s2);
extern const char *find_word(const char *s, enum dir_t dir);
extern void reset_dicts(void);
extern int run_table_tests(void);

#endif /* __TABLE_H__ */
