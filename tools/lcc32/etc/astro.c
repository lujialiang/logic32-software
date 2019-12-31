/* Sun4s running Solaris at Princeton University */

#include <string.h>

char *cpp[] = { "/usr/p-ton/lib/gcc-lib/sparc-sun-solaris2/2.5.8/cpp",
	"-undef", "-Dsparc", "-D__sparc__", "-Dsun", "-D__sun__",
	"$1", "$2", "$3", 0 };
char *include[] = { "-I/u/drh/include", "-I/usr/include",
	"-I/opt/SUNWspro/SC2.0.1/include/cc", 0 };
char *com[] = { "/u/drh/bin/rcc", "-target=sparc-solaris",
	"$1", "$2", "$3", 0 };
char *as[] = { "/usr/ccs/bin/as", "-Qy", "-s", "-o", "$3", "$1", "$2", 0 };
char *ld[] = { "/usr/ccs/bin/ld", "-o", "$3", "$1",
	"/opt/SUNWspro/SC2.0.1/crti.o",	"/opt/SUNWspro/SC2.0.1/crt1.o",
	"/opt/SUNWspro/SC2.0.1/__fstd.o", "/opt/SUNWspro/SC2.0.1/values-xt.o",
	"$2", "-Y", "P,/opt/SUNWspro/SC2.0.1:/usr/ccs/lib:/usr/lib", "-Qy",
	"-lm", "-lc", "/opt/SUNWspro/SC2.0.1/crtn.o", 0 };

int option(arg) char *arg; {
	return 0;
}
