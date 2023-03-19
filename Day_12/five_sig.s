	.file	"five_sig.cpp"
	.text
	.type	_ZZ4mainENUliE_4_FUNEi, @function
_ZZ4mainENUliE_4_FUNEi:
.LFB2147:
	.cfi_startproc
	endbr64
	movl	$0, loop(%rip)
	ret
	.cfi_endproc
.LFE2147:
	.size	_ZZ4mainENUliE_4_FUNEi, .-_ZZ4mainENUliE_4_FUNEi
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"count is %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2145:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	call	alarm@PLT
	leaq	_ZZ4mainENUliE_4_FUNEi(%rip), %rsi
	movl	$14, %edi
	call	signal@PLT
	cmpl	$0, loop(%rip)
	je	.L3
.L4:
	jmp	.L4
.L3:
	movl	$0, %ecx
	leaq	.LC0(%rip), %rdx
	movl	$1, %esi
	movq	stdout(%rip), %rdi
	movl	$0, %eax
	call	__fprintf_chk@PLT
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2145:
	.size	main, .-main
	.globl	loop
	.data
	.align 4
	.type	loop, @object
	.size	loop, 4
loop:
	.long	1
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
