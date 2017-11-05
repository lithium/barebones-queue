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


char *Hexstring(char *buf, int buf_size, uint64_t value)
{
	uint64_t mask = 0xF000000000000000;
	int i=0;

	while (i < buf_size && mask) {
		char c = (value & mask)	>> (15 - i)*4;
		buf[i] = (c < 10) ? '0'+c : 'a'+(c-10);

		i = i+1;
		mask = mask >> 4;
	}

	return buf;
}
