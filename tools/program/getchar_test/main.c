
#include <stdio.h>
#include <string.h>
#include <soc.h>

int main(void)
{
	int i = 0;
	char key;
	while( (key = _getchar()) != 0x1b ) {
		i++;
		printf("#%d %d, 0x%x\n", i, key, key);
	}
	return 0;
}
