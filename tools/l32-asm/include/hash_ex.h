#ifndef _HASH_EX_INCLUDE
#define _HASH_EX_INCLUDE

#include "link.h"
#include "a_out.h"

//#define NHASH 1024
#define NHASH (1024*10)

struct glosym **hash_ex_init(void);
void hash_ex_deinit(void);
struct glosym *hash_ex_in(struct nlist *psym);
struct glosym *hash_ex_lookup(char *name);
void dump_global_symbols(int);

#endif
