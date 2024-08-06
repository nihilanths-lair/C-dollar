	.file	"chess_board.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "\0"
.LC1:
	.ascii "compiler.cfg\0"
.LC2:
	.ascii "\324\340\351\353 \357\363\361\362\356\351.\0"
.LC3:
	.ascii "\324\340\351\353 \361\356\344\345\360\346\350\362 \347\340\357\350\361\374.\0"
.LC4:
	.ascii "\324\340\351\353 \347\340\352\360\373\362.\0"
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
	leaq	.LC1(%rip), %rdx
	movq	%rax, %rcx
	call	fop
	cmpb	$-1, %al
	jne	.L2
	movl	$1, %eax
	jmp	.L3
.L2:
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	call	fgetc
	movb	%al, -9(%rbp)
	cmpb	$-1, -9(%rbp)
	jne	.L4
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movq	-8(%rbp), %rax
	movq	%rax, %rdx
	movl	$116, %ecx
	call	fputc
	movq	-8(%rbp), %rax
	movq	%rax, %rdx
	movl	$101, %ecx
	call	fputc
	movq	-8(%rbp), %rax
	movq	%rax, %rdx
	movl	$115, %ecx
	call	fputc
	movq	-8(%rbp), %rax
	movq	%rax, %rdx
	movl	$116, %ecx
	call	fputc
	jmp	.L5
.L4:
	leaq	.LC3(%rip), %rax
	movq	%rax, %rcx
	call	printf
.L5:
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	leaq	.LC4(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
.L3:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC5:
	.ascii "rb\0"
.LC6:
	.ascii "wb\0"
.LC7:
	.ascii "\315\345 \361\354\356\343 \361\356\347\344\340\362\374 \364\340\351\353.\0"
	.align 8
.LC8:
	.ascii "\324\340\351\353 \361\356\347\344\340\355 \350 \356\362\352\360\373\362 \355\340 \347\340\357\350\361\374.\0"
	.align 8
.LC9:
	.ascii "\315\345 \361\354\356\343 \356\362\352\360\373\362\374 \364\340\351\353 \355\340 \367\362\345\355\350\345.\0"
.LC10:
	.ascii "\324\340\351\353 \357\345\360\345\356\362\352\360\373\362 \355\340 \367\362\345\355\350\345.\0"
.LC11:
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
	movq	%rdx, 24(%rbp)
	movq	24(%rbp), %rax
	leaq	.LC5(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L7
	movq	24(%rbp), %rax
	leaq	.LC6(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L8
	leaq	.LC7(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$-1, %eax
	jmp	.L9
.L8:
	leaq	.LC8(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movq	16(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	leaq	.LC4(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movq	24(%rbp), %rax
	leaq	.LC5(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L10
	leaq	.LC9(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$-1, %eax
	jmp	.L9
.L10:
	leaq	.LC10(%rip), %rax
	movq	%rax, %rcx
	call	puts
	jmp	.L11
.L7:
	leaq	.LC11(%rip), %rax
	movq	%rax, %rcx
	call	puts
.L11:
	movl	$0, %eax
.L9:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (Rev6, Built by MSYS2 project) 13.2.0"
	.def	setlocale;	.scl	2;	.type	32;	.endef
	.def	fgetc;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	fputc;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	fclose;	.scl	2;	.type	32;	.endef
	.def	fopen;	.scl	2;	.type	32;	.endef
