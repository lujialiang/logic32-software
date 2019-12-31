/* SGI big endian MIPSes running IRIX 5.2 at Princeton University */

#include <string.h>

char *cpp[] = { "/usr/local/lib/cpp", "-D__STDC__=1",
	"-DLANGUAGE_C",
	"-DMIPSEB",
	"-DSYSTYPE_SVR4",
	"-D_CFE",
	"-D_LANGUAGE_C",
	"-D_MIPSEB",
	"-D_MIPS_FPSET=16",
	"-D_MIPS_ISA=_MIPS_ISA_MIPS1",
	"-D_MIPS_SIM=_MIPS_SIM_ABI32",
	"-D_MIPS_SZINT=32",
	"-D_MIPS_SZLONG=32",
	"-D_MIPS_SZPTR=32",
	"-D_SVR4_SOURCE",
	"-D_SYSTYPE_SVR4",
	"-D__host_mips",
	"-D__mips=1",
	"-D__sgi",
	"-D__unix",
	"-Dhost_mips",
	"-Dmips",
	"-Dsgi",
	"-Dunix",
	"$1", "$2", "$3", 0 };
char *com[] =  { "/usr/local/lib/rcc", "-target=mips-irix", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ansi", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "-non_shared", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-non_shared", "-o", "$3", "/usr/lib/nonshared/crt1.o",
	"$1", "$2", "", "",
	"-elf", "-_SYSTYPE_SVR4", "-Wx,-G", "8", "-g0",
	"-lc", "-lm", "/usr/lib/nonshared/crtn.o", 0
};

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0)
		ld[4] = "/usr/lib/nonshared/mcrt1.o";
	else if (strcmp(arg, "-b") == 0)
		ld[7] = "/usr/local/lib/bbexit.o";
	else
		return 0;
	return 1;
}
