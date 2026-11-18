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


int create_rules(unsigned char *rule_tape,
                 const unsigned char *patterns,
                 const unsigned char *replacements,
                 uint32_t rules_count,
                 uint32_t rule_length) {
    const uint32_t rule_size = 64;

    if (rule_length == 0 || rule_length > 16) {
        return -1;
    }

    const uint32_t mask = rule_length == 16
        ? 0xffffu
        : (1u << rule_length) - 1u;

    for (uint32_t i = 0; i < rules_count; i++) {
        unsigned char *rule = rule_tape + i * rule_size;
        const uint32_t source_offset = i * rule_length;
        const uint32_t flags = 0;

        memcpy(rule, patterns + source_offset, rule_length);
        memcpy(rule + 16, replacements + source_offset, rule_length);
        memcpy(rule + 32, &mask, sizeof(mask));
        memcpy(rule + 36, &rule_length, sizeof(rule_length));
        memcpy(rule + 40, &rule_length, sizeof(rule_length));
        memcpy(rule + 44, &flags, sizeof(flags));
    }

    return 0;
}

int main(void) {
    /* markovNAM rewrites the working tape in place. */
    unsigned char buffer[32] = "abc xyz";
    const unsigned long position = 7;
    const unsigned char patterns[] = {'a', 'b', 'x', 'y'};
    const unsigned char replacements[] = {'x', 'y', 'z', 'z'};
    unsigned char rules[128] = {0};

    const unsigned long rules_length=2;
    if (create_rules(rules, patterns, replacements, rules_length, 2) != 0) {
        return 1;
    }

    printf("tape before is %s\n", buffer);
    const int counter = markovNAM(buffer, position, rules, rules_length);
    printf("counter is %d\n", counter);
    printf("tape is %s\n", buffer);

    return 0;
}


/*  rdi → buffer */
/* rsi → length */
/* rdx → rules */
/* rcx → rule_count */
/* r8  → capacity */
/* r9  → дополнительные флаги */
