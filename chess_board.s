	.file	"chess_board.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "\0"
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
	.ascii "\324\340\351\353 \356\362\352\360\373\362 \355\340 \367\362\345\355\350\345.\0"
.LC7:
	.ascii "A | cfg[] = \"%s\"\12\0"
.LC8:
	.ascii "B | cfg[] = \"%s\"\12\0"
.LC9:
	.ascii "\300\360\343\363\354\345\355\362\373 \361 \352\356\355\361\356\353\350\0"
.LC10:
	.ascii "\300\360\343\363\354\345\355\362\373 \361 \352\356\355\361\356\353\350.\0"
	.align 8
.LC11:
	.ascii "\312\356\353\350\367\345\361\362\342\356 \357\345\360\345\344\340\355\355\373\365 \340\360\343\363\354\345\355\362\356\342 \364\363\355\352\366\350\350 main: %d.\12\0"
.LC12:
	.ascii "\300\360\343\363\354\345\355\362 %d: %s\12\0"
.LC13:
	.ascii "\300\360\343\363\354\345\355\362\373 \361 \364\340\351\353\340\0"
.LC14:
	.ascii "\300\360\343\363\354\345\355\362\373 \361 \364\340\351\353\340.\0"
.LC15:
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
	subq	$80, %rsp
	.seh_stackalloc	80
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	call	__main
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdx
	movl	$0, %ecx
	call	setlocale
	movq	$0, -8(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	fopen
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L2
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	fopen
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L3
	leaq	.LC4(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$-1, %eax
	jmp	.L4
.L3:
	leaq	.LC5(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movabsq	$-941843941183721280, %rax
	movabsq	$-1447378466093588229, %rdx
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	$224, -32(%rbp)
	movq	$0, -24(%rbp)
	movq	-8(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rax, %rcx
	call	fputs
	jmp	.L5
.L2:
	leaq	.LC6(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movb	$0, -48(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC7(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movq	-8(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rdx, %r8
	movl	$32, %edx
	movq	%rax, %rcx
	call	fgets
	leaq	-48(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC8(%rip), %rax
	movq	%rax, %rcx
	call	printf
	leaq	-48(%rbp), %rax
	leaq	.LC9(%rip), %rdx
	movq	%rax, %rcx
	call	strcmp
	testl	%eax, %eax
	jne	.L6
	leaq	.LC10(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	16(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC11(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, -12(%rbp)
	jmp	.L7
.L8:
	movl	-12(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movl	-12(%rbp), %eax
	movq	%rdx, %r8
	movl	%eax, %edx
	leaq	.LC12(%rip), %rax
	movq	%rax, %rcx
	call	printf
	addl	$1, -12(%rbp)
.L7:
	movl	-12(%rbp), %eax
	cmpl	16(%rbp), %eax
	jl	.L8
	jmp	.L5
.L6:
	leaq	-48(%rbp), %rax
	leaq	.LC13(%rip), %rdx
	movq	%rax, %rcx
	call	strcmp
	testl	%eax, %eax
	jne	.L5
	leaq	.LC14(%rip), %rax
	movq	%rax, %rcx
	call	puts
.L5:
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	leaq	.LC15(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
.L4:
	addq	$80, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (Rev6, Built by MSYS2 project) 13.2.0"
	.def	setlocale;	.scl	2;	.type	32;	.endef
	.def	fopen;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	fputs;	.scl	2;	.type	32;	.endef
	.def	fgets;	.scl	2;	.type	32;	.endef
	.def	strcmp;	.scl	2;	.type	32;	.endef
	.def	fclose;	.scl	2;	.type	32;	.endef
