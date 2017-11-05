#ifndef BAREBONES_STRING_H
#define BAREBONES_STRING_H

#include <inttypes.h>


void *Memset(void *b, int c, int len);
void *Memcpy(void *dest, void *src, int len);

int StringEq(char *s1, char *s2, int len);



char *Hexstring(char *buf, int buf_len, uint64_t value);
#endif
