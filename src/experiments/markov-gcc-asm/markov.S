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


.section .text
.global markovNAM
.type markovNAM, @function

# int markovNAM(buffer,position)
# %rdi = лента буфер
# %rsi = длина ленты
# %rdx = будущие правила
# %rcx = количество правил 	
# %r11 = индекс текущего правила 
	
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
