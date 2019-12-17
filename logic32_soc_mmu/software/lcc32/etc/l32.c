/* Sun-4s running Sun UNIX 4.1.1 at Bell Labs */

#include <string.h>

#define TOOLCHAIN_ROOT "/usr/local/bin/"

#if 1
char *cpp[] = { TOOLCHAIN_ROOT "lcc-cpp", "-DL32", "$1", "$2", "$3", 0 };
#else
char *cpp[] = { "/usr/bin/cpp", "-DL32", "$1", "$2", "$3", 0 };
#endif

char *include[] = { "-I" TOOLCHAIN_ROOT "include/l32", 0 };
char *com[] = { TOOLCHAIN_ROOT "l32-rcc", "-target=l32", "$1", "$2", "$3", 0 };
char *as[] = { TOOLCHAIN_ROOT "l32-asm", "$1", "$2", "-o", "$3", 0 };
#if 0
char *ld[] = { TOOLCHAIN_ROOT "l32-link" , TOOLCHAIN_ROOT "l32-toolchain/lib/crt0.o", "$1", "$2",0 };
#else
char *ld[] = { TOOLCHAIN_ROOT "l32-link" , "$1", "$2",0 };
#endif

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		ld[13] = "-lg";
	else if (strcmp(arg, "-p") == 0) {
		ld[8]  = "/usr/lib/mcrt0.o";
		ld[14] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0) {
		ld[8]  = "/usr/lib/gcrt0.o";
		ld[14] = "-lc_p";
	} else if (strcmp(arg, "-b") == 0)
		ld[11] = "/usr/graphics/LIB.sun4/bbexit.o";
	else
		return 0;
	return 1;
}
