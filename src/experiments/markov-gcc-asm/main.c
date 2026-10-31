#include <stdint.h>
#include <stdio.h>
#include <string.h>
extern int simd_match16(
    const unsigned char *position,
    const unsigned char *pattern,
    uint32_t mask
);


extern int markovNAM(
    const unsigned char *buffer,
    const unsigned long position
);

int main(void) {
    const unsigned char buffer[32] = "abc xyz";
    const unsigned long position = 7;
    const int counter = markovNAM(buffer, position);
    printf("counter is %d\n", counter);

    return 0;
}


/*  rdi → buffer */
/* rsi → length */
/* rdx → rules */
/* rcx → rule_count */
/* r8  → capacity */
/* r9  → дополнительные флаги */
