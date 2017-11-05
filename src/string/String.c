#include "string/String.h"


int StringEq(char *s1, char *s2, int len)
{
	while (len--) {
		char c= *s1 - *s2;
		if (c != 0) {
			return 0;
		}
		s1++;
		s2++;
	}
	return 1;
}


void *Memset(void *b, int c, int len)
{
	while (len--) {
		*(char *)b++ = c;
	}
	return b;
}


void *Memcpy(void *dst, void *src, int len) {
	while (len--) {
		*(uint8_t *)(dst + len) = *(uint8_t *)(src +len);
	}
	return dst;
}


char *Hexstring(char *buf, int buf_size, uint64_t value)
{
	uint64_t mask = 0xF000000000000000;
	int i=0;

	while (i < buf_size && mask) {
		uint64_t c = (value & mask)	>> (15 - i)*4;
		buf[i] = (c < 10) ? '0'+c : 'a'+(c-10);

		i = i+1;
		mask = mask >> 4;
	}
	buf[i] = 0;

	return buf;
}
