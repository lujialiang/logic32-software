#include <stddef.h>

size_t strlen(const char *str)
{
	size_t l = 0;
	while(*str++) {
		l++;
	}
	return l;
}

int strcmp(const char *s1, const char *s2)
{
	while(*s1 && *s1 == *s2) {
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	while(*s1 && *s1 == *s2 && n--) {
		s1++;
		s2++;
	}
	return n ? *s1 - *s2 : 0;
}

/*
char *stpcpy(char *dest, const char *src)
{
	while(*dest++ = *src++);
	return dest;
}
*/

//char *stpncpy(char *dest, const char *src, size_t n);

char *strcpy(char *dest, const char *src)
{
	char *ret = dest;
	while(*dest++ = *src++);
	return ret;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	char *ret = dest;
	while(n--) {
		*dest++ = *src;
		if(*src) {
			src++;
		}
	}
	return ret;
}

char *strcat(char *dest, const char *src)
{
	char *ret = dest;
	dest += strlen(dest);
	while(*dest++ = *src++);
	return ret;
}

char *strncat(char *dest, const char *src, size_t n)
{
	char *ret = dest;
	dest += strlen(dest);
	while(n-- && (*dest++ = *src++));
	*dest = 0;
	return ret;
}

char *strchr(const char *str, int c)
{
	while(*str) {
		if(*str == c ) {
			return (char *)str;
		}
		str++;
	}
	return NULL;
}

char *strstr(const char *s1, const char *s2)
{
	const char *p = s1;
	const size_t len = strlen(s2);
	for(;(p=strchr(p, *s2))!=0;p++) {
		if(strncmp(p, s2, (size_t)len) == 0) {
			return (char *)p;
		}
	}
	return NULL;
}

void *memset(void *s, int c, size_t n)
{
	char *p = s;
	while(n--) {
		*p++ = c;
	}
	return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	char *p1 = dest;
	const char *p2 = src;
	while(n--) {
		*p1++ = *p2++;
	}
	return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
    char *p1 = dest;
    const char *p2 = src;
    size_t m = n;
    if(p1 > p2) {
        while(n) {
            p1[--n] = p2[--m];
        }   
    }   
    else if( p1 < p2 ) {
        while(n--) {
            *p1++ = *p2++;
        }   
    }
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	const char *p1 = s1, *p2 = s2;
	while (*p1 == *p2 && n--) p1++, p2++;
	return n ? *p1 - *p2 : 0;
}
