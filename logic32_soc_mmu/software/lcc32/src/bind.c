#include "c.h"
extern Interface nullIR,   symbolicIR;
extern Interface mipsebIR, mipselIR;
//extern Interface sparcIR,  solarisIR;
//extern Interface x86IR;
extern Interface l32IR;
Binding bindings[] = {
	"symbolic",      &symbolicIR,
	"mips-eb",     &mipsebIR,
	"mips-el",   &mipselIR,
	//"sparc-sun",     &sparcIR,
	//"sparc-solaris", &solarisIR,
	//"x86-dos",       &x86IR,
	"l32",       &l32IR,
	"null",          &nullIR,
	NULL,            NULL
};
