	.file	"chess_board.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdx
	movl	$0, %ecx
	call	setlocale
	movq	$0, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	call	fop
	cmpb	$-1, %al
	jne	.L2
	movl	$1, %eax
	jmp	.L3
.L2:
	movl	$0, %eax
.L3:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "rb\0"
.LC2:
	.ascii "compiler.cfg\0"
.LC3:
	.ascii "wb\0"
.LC4:
	.ascii "\315\345 \361\354\356\343 \361\356\347\344\340\362\374 \364\340\351\353.\0"
	.align 8
.LC5:
	.ascii "\324\340\351\353 \361\356\347\344\340\355 \350 \356\362\352\360\373\362 \355\340 \347\340\357\350\361\374.\0"
.LC6:
	.ascii "\324\340\351\353 \347\340\352\360\373\362.\0"
	.align 8
.LC7:
	.ascii "\315\345 \361\354\356\343 \356\362\352\360\373\362\374 \364\340\351\353 \355\340 \367\362\345\355\350\345.\0"
.LC8:
	.ascii "\324\340\351\353 \357\345\360\345\356\362\352\360\373\362 \355\340 \367\362\345\355\350\345.\0"
.LC9:
	.ascii "\324\340\351\353 \356\362\352\360\373\362 \355\340 \367\362\345\355\350\345.\0"
	.text
	.globl	fop
	.def	fop;	.scl	2;	.type	32;	.endef
	.seh_proc	fop
fop:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L5
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L6
	leaq	.LC4(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$-1, %eax
	jmp	.L7
.L6:
	leaq	.LC5(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movq	16(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	leaq	.LC6(%rip), %rax
	movq	%rax, %rcx
	call	puts
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L8
	leaq	.LC7(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$-1, %eax
	jmp	.L7
.L8:
	leaq	.LC8(%rip), %rax
	movq	%rax, %rcx
	call	puts
	jmp	.L9
.L5:
	leaq	.LC9(%rip), %rax
	movq	%rax, %rcx
	call	puts
.L9:
	movq	16(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	leaq	.LC6(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
.L7:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (Rev6, Built by MSYS2 project) 13.2.0"
	.def	setlocale;	.scl	2;	.type	32;	.endef
	.def	fopen;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	fclose;	.scl	2;	.type	32;	.endef
