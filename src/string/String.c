#include "string/String.h"


int StringEq(char *s1, char *s2, int len)
{
	while (len--) {
		int ret = *s1 - *s2;
		if (ret != 0) {
			return ret;
		}
	}
	return 0;
}


void *Memset(void *b, int c, int len)
{
	while (len--) {
		*(char *)b++ = c;
	}
}
