.section .text
.global simd_match16
.type simd_match16, @function

# int simd_match16(position, pattern, mask)
# %rdi = адрес текущей позиции в рабочем буфере
# %rsi = адрес 16-байтового образца
# %edx = маска значимых байтов образца
# %eax = 1 при совпадении, иначе 0
simd_match16:
    movdqu  (%rdi), %xmm0
    movdqu  (%rsi), %xmm1
    pcmpeqb %xmm1, %xmm0
    pmovmskb %xmm0, %eax
    andl    %edx, %eax
    cmpl    %edx, %eax
    sete    %al
    movzbl  %al, %eax
    ret

.size simd_match16, .-simd_match16

.section .note.GNU-stack,"",@progbits
