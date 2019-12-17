#ifndef _SETJMP_H_INCLUDE_
#define _SETJMP_H_INCLUDE_

struct __jmp_buf_tag
{
	unsigned int regs[32];
};

typedef struct __jmp_buf_tag jmp_buf[1];

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#endif
