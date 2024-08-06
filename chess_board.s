	.file	"chess_board.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "\0"
.LC1:
	.ascii "compiler.cfg\0"
	.align 8
.LC2:
	.ascii "\312\356\353\350\367\345\361\362\342\356 \357\345\360\345\344\340\355\355\373\365 \340\360\343\363\354\345\355\362\356\342 \364\363\355\352\366\350\350 main: %d.\12\0"
.LC3:
	.ascii "\300\360\343\363\354\345\355\362 %d: %s\12\0"
.LC4:
	.ascii "\324\340\351\353 \357\363\361\362\356\351.\0"
.LC5:
	.ascii "\324\340\351\353 \361\356\344\345\360\346\350\362 \347\340\357\350\361\374.\0"
.LC6:
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
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	call	__main
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdx
	movl	$0, %ecx
	call	setlocale
	movq	$0, -16(%rbp)
	movq	-16(%rbp), %rax
	leaq	.LC1(%rip), %rdx
	movq	%rax, %rcx
	call	fop
	cmpb	$-1, %al
	jne	.L2
	movl	$1, %eax
	jmp	.L3
.L2:
	movl	16(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, -4(%rbp)
	jmp	.L4
.L5:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movl	-4(%rbp), %eax
	movq	%rdx, %r8
	movl	%eax, %edx
	leaq	.LC3(%rip), %rax
	movq	%rax, %rcx
	call	printf
	addl	$1, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	16(%rbp), %eax
	jl	.L5
	movq	-16(%rbp), %rax
	movq	%rax, %rcx
	call	fgetc
	movb	%al, -17(%rbp)
	cmpb	$-1, -17(%rbp)
	jne	.L6
	leaq	.LC4(%rip), %rax
	movq	%rax, %rcx
	call	puts
	jmp	.L7
.L6:
	leaq	.LC5(%rip), %rax
	movq	%rax, %rcx
	call	printf
.L7:
	movq	-16(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	leaq	.LC6(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
.L3:
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC7:
	.ascii "rb\0"
.LC8:
	.ascii "wb\0"
.LC9:
	.ascii "\315\345 \361\354\356\343 \361\356\347\344\340\362\374 \364\340\351\353.\0"
	.align 8
.LC10:
	.ascii "\324\340\351\353 \361\356\347\344\340\355 \350 \356\362\352\360\373\362 \355\340 \347\340\357\350\361\374.\0"
	.align 8
.LC11:
	.ascii "\315\345 \361\354\356\343 \356\362\352\360\373\362\374 \364\340\351\353 \355\340 \367\362\345\355\350\345.\0"
.LC12:
	.ascii "\324\340\351\353 \357\345\360\345\356\362\352\360\373\362 \355\340 \367\362\345\355\350\345.\0"
.LC13:
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
	leaq	.LC7(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L9
	movq	24(%rbp), %rax
	leaq	.LC8(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L10
	leaq	.LC9(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$-1, %eax
	jmp	.L11
.L10:
	leaq	.LC10(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movq	16(%rbp), %rax
	movq	%rax, %rdx
	movl	$116, %ecx
	call	fputc
	movq	16(%rbp), %rax
	movq	%rax, %rdx
	movl	$101, %ecx
	call	fputc
	movq	16(%rbp), %rax
	movq	%rax, %rdx
	movl	$115, %ecx
	call	fputc
	movq	16(%rbp), %rax
	movq	%rax, %rdx
	movl	$116, %ecx
	call	fputc
	movq	16(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	leaq	.LC6(%rip), %rax
	movq	%rax, %rcx
	call	puts
	movq	24(%rbp), %rax
	leaq	.LC7(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 16(%rbp)
	cmpq	$0, 16(%rbp)
	jne	.L12
	leaq	.LC11(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$-1, %eax
	jmp	.L11
.L12:
	leaq	.LC12(%rip), %rax
	movq	%rax, %rcx
	call	puts
	jmp	.L13
.L9:
	leaq	.LC13(%rip), %rax
	movq	%rax, %rcx
	call	puts
.L13:
	movl	$0, %eax
.L11:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (Rev6, Built by MSYS2 project) 13.2.0"
	.def	setlocale;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	fgetc;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	fclose;	.scl	2;	.type	32;	.endef
	.def	fopen;	.scl	2;	.type	32;	.endef
	.def	fputc;	.scl	2;	.type	32;	.endef
