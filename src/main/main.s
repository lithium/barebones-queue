

.section .text

.global main
.type main, @function
main:

	mov		$1, %eax
	ret

.size main, . - main
