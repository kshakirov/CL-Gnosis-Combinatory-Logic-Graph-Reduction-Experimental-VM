.text
.globl curry_app
.type curry_app, @function

curry_app:
	mov %rsi, %rax
	imul %rdx,%rax
	add  %rdi,%rax
	movl $2 , (%rax)
	movl %ecx, 4(%rax)
	movl %r8d, 8(%rax)			

	

    ret                         # Возврат

.size curry_app, .-curry_app

.section .note.GNU-stack,"",@progbits
