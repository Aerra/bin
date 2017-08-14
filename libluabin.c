#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#include <assert.h>
#include "portable_endian.h"
#include "xd.h"

char * bin_xd(char *data, size_t size, xd_conf *cf) {
	return xd(data, size, cf);
}

uint16_t bin_htobe16 (uint16_t x) { return htobe16(x); }
uint16_t bin_htole16 (uint16_t x) {	return htole16(x); }
uint16_t bin_be16toh (uint16_t x) { return be16toh(x); }
uint16_t bin_le16toh (uint16_t x) {	return le16toh(x); }

uint32_t bin_htobe32 (uint32_t x) { return htobe32(x); }
uint32_t bin_htole32 (uint32_t x) {	return htole32(x); }
uint32_t bin_be32toh (uint32_t x) { return be32toh(x); }
uint32_t bin_le32toh (uint32_t x) {	return le32toh(x); }

uint64_t bin_htobe64 (uint64_t x) { return htobe64(x); }
uint64_t bin_htole64 (uint64_t x) {	return htole64(x); }
uint64_t bin_be64toh (uint64_t x) { return be64toh(x); }
uint64_t bin_le64toh (uint64_t x) {	return le64toh(x); }

uint64_t decode_reb(unsigned char *p) {
	uint64_t val = (*p & 0x7f);
	p++;
	uint32_t i = 1;
	for (; *(p-1) & 0x80; i++, p++) {
		assert(i < 9);
		val += (*p & 0x7f) << (i * 7);
	}
	return val;
}

void encode_reb(uint64_t n, char *buf) {
	if (n <= 0x7f) {
		*buf = (char) n;
	} else {
		char *ptr = buf;
		while(n) {
			*ptr++ = (n & 0x7f) | ( n > 0x7f ? 0x80 : 0 );
			n >>= 7;
		}
	}
	return;
}

void encode_ber(uint64_t n, char *buf, size_t numsize) {
	char *ptr = buf;
	int i = numsize - 1;
	for (; i > 0; i--) {
		*ptr++ = (n >> 7*i) & 0x80;
	}
	*ptr = n & 0x7f;
	return;
}

char * bin_hex(unsigned char *p, size_t size) {
	char *rv = malloc(size*2+1);
	char *r = rv;
	if (!rv) {
		fprintf(stderr,"Can't allocate memory\n");
		return NULL;
	}
	unsigned char *	e = p + size;
	for (; p<e; p++) {
		snprintf(rv, 3, "%02X", *p);
		rv+=2;
	}
	*rv = 0;
	return r;
}
