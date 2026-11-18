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

#r9 + 0  → образец
#r9 + 16 → замена
#r9 + 32 → готовая маска длины

	
.macro TRY_RULE tape, tape_position, rules, rule_idx
    movq    \tape, %r8
    add \tape_position, %r8
    movq \rule_idx, %r9
    shl $6, %r9  #here we have rule index which we multiply by 64 it is a rule fixed size
    add \rules, %r9	 # here we add rules offset 
    movdqu  (%r8), %xmm0  # here you have the to search where
    movdqu  (%r9), %xmm1  #here you can see
    pcmpeqb %xmm1, %xmm0  # here the parallel cmp by byte 
    pmovmskb %xmm0, %eax  #here we move mask to eax 
    andl    32(%r9), %eax   # here we do AND with rules and masks to see the real length of cmp
    cmpl    32(%r9), %eax # compare long - l, means with sign 
    sete    %al    # set flag 1 or 0 if the previous comparison is 
    movzbl  %al, %eax # move byte and expand it with zeros
.endm




.section .text
.global markovNAM
.type markovNAM, @function

# int markovNAM(buffer,position)
# %rdi = лента буфер
# %rsi = длина ленты
# %rdx = будущие правила
# %rcx = количество правил 	
# %r11 = индекс текущего правила 
# %r10 = текущая позиция на ленте	
markovNAM:
position_loop:
	
	#movq %rsi, %rax
	movq $0, %r10
external_loop:
	cmp %rsi, %r10		
	jae exit
	movq $0,%r11 # index of first rule		
inner_loop:
	# here run the rule macro or simd_match16
	# here check if 1 loop back from the 0th rule if not go with anoter rule
	cmp %rcx, %r11
	jae next_position
	TRY_RULE %rdi, %r10, %rdx, %r11		
	add $1, %r11
	jmp inner_loop
next_position:

	add $1, %r10
	jmp external_loop

exit:
    movq %r10,%rax		
    ret	
	
	


.size markovNAM, .-markovNAM

.section .note.GNU-stack,"",@progbits
