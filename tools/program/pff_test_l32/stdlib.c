
extern void halt(void);

void exit(int status)
{
	halt();
}
