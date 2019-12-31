/* DOS commands for use with Borland Turbo Assembler and C++ 4.0, and PowerPack */

#include <string.h>

char *tempdir[]	= { "c:\\tmp\\", 0 };
char *shell[]	= { "c:\\dos\\command.com", "/C", "$1", 0 };
char *delete[]	= { "del", "$1", 0 };
char *cpp[]	= { "cpp", "-D__STDC__=1 -D__cdecl= -D__DPMI32__ -D__FLAT__", "$1", "$2", "$3", 0 };
char *include[]	= { "-I/bc4/include", 0 };
char *com[]	= { "rcc", "-target=x86-dos", "$1", "$2", "$3", 0 };
char *as[]	= { "c:\\tasm\\bin\\tasm32", "/t /ml", "$2", ",", "$3", 0 };
char *ld[]	= { "c:\\bc4\\bin\\tlink32", "/c /Tpe /ax /x c0x32.obj",
			"$1", "$2", ",", "$3", ",,dpmi32.lib cw32.lib", 0 };

int option(arg) char *arg; {
	return 0;
}
