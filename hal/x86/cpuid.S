.global cpuid_supported

cpuid_supported:
	pushfl
	pop %ecx
	mov %ecx, %eax 
	 
	xor $0x200000, %eax
	 
	push %eax
	popfl

	pushfl
	pop %eax
	 
	xor %ecx, %eax
	je unsupported
	jmp supported

		unsupported:
			mov $0x0, %eax

		supported:
			mov $0x1, %eax

	ret
