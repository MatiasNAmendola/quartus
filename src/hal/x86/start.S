.section multiboot
       
#define MBH_MAGIC 0x1badb002    
#define MBH_FLAGS 0x0     
#define MBH_CHECKSUM -(MBH_MAGIC + MBH_FLAGS)  
       
.align 4      
.int MBH_MAGIC      
.int MBH_FLAGS      
.int MBH_CHECKSUM     

.section .text      

.global start      

.extern ctors
.extern init

start:       
	mov $stack, %esp    

	push %eax
	push %ebx

	call ctors     
	call init     

halt:       
	cli
	hlt
	jmp halt

.section .bss      
.align 4096
.space 8192      
stack:
