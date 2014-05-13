.section .text.boot     

.global start      

.global __aeabi_idivmod
.global __aeabi_idiv
.global __aeabi_l2d

.extern ctors
.extern init

__aeabi_idivmod:
	b halt

__aeabi_idiv:
	b halt

__aeabi_l2d:
	b halt

start:       
	mov 	sp, #0x8000   

	ldr 	r4, =bss_start
	ldr 	r9, =bss_end

	mov 	r5, #0
	mov 	r6, #0
	mov 	r7, #0
	mov 	r8, #0
	b   	2f

1:
	stmia 	r4!, {r5-r8}

2:
	cmp 	r4, r9
	blo 	1b

	stmfd 	sp!, {r0, r1}

	ldr 	r3, =ctors
	blx 	r3

	ldr 	r3, =init
	blx 	r3     

halt:
	wfe 
	b 	halt
