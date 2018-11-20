// MTEZX MK3
// Copyleft 2017 by The Mojon Twins

// library

extern const unsigned char m0_c [0];
#asm
	._m0_c
		BINARY "../ports/cpc/bin/m0.c.bin"
#endasm

const unsigned char * library [] = {
	m0_c
};

#define M0_C 0x00

