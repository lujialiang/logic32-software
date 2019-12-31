
int strlen(char *s)
{
	int i = 0;
	while(*s) {
		i++;
		s++;
	}
	return i;
}

int strcmp(const char *s1, const char *s2)
{
	while(*s1 == *s2) {
		if( *s1 == '\0' ) {
			return 0;
		}
		s1++;
		s2++;
	}
	return -1;
}

int strncmp(const char *s1, const char *s2, unsigned n)
{
	while(*s1 == *s2 && n > 0) {
		if( *s1 == '\0' ) {
			return 0;
		}
		s1++;
		s2++;
		n--;
	}
	return -1;
}

#if 0
void *memset(void *s, unsigned char ch, unsigned n)
{
	unsigned char *p = (unsigned char *)s;
	while(n) {
		*p++ = ch;
		n--;
	}
	return s;
}
#endif
