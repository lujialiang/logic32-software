#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "my_dbg.h"

#if 0
#define BIT(a,x) (((a & (1<<x)) >> x) & 1)
#else
inline int BIT(uint32_t,int);
#endif

void error(const char *,int);
void warning(const char *,int);

extern bool N;
extern bool Z;
extern bool V;
extern bool C;

#ifdef __cplusplus
}
#endif

#endif // _COMMON_H_
