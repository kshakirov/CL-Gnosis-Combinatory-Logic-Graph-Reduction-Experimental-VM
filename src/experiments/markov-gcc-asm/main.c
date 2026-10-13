#include <stdint.h>
#include <stdio.h>

extern int simd_match16(
    const unsigned char *position,
    const unsigned char *pattern,
    uint32_t mask
);

int main(void) {
    /* SIMD-загрузка читает 16 байт, поэтому у буфера есть безопасный запас. */
    const unsigned char buffer[32] = "abc xyz";
    const unsigned char pattern[16] = {'a', 'b', 'c'};
    const uint32_t mask = 0x0007;

    const int match_at_0 = simd_match16(&buffer[0], pattern, mask);
    const int match_at_1 = simd_match16(&buffer[1], pattern, mask);

    printf("position 0: %s\n", match_at_0 ? "match" : "no match");
    printf("position 1: %s\n", match_at_1 ? "match" : "no match");

    return match_at_0 == 1 && match_at_1 == 0 ? 0 : 1;
}
