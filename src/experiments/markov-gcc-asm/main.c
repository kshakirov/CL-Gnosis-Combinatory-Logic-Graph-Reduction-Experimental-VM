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
    const unsigned long position,
    const unsigned char *rules,
    const unsigned long rules_length
    
);

int main(void) {
    const unsigned char buffer[32] = "abc xyz";
    const unsigned long position = 7;
    const  unsigned char rule[16]={'a','x'};
    const unsigned long rules_length=2;
    const int counter = markovNAM(buffer, position, rule, rules_length);
    printf("counter is %d\n", counter);

    return 0;
}


/*  rdi → buffer */
/* rsi → length */
/* rdx → rules */
/* rcx → rule_count */
/* r8  → capacity */
/* r9  → дополнительные флаги */
