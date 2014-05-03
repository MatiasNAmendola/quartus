.section multiboot					//Multiboot-Header
							//	|
#define MBH_MAGIC 0x1badb002				//	|
#define MBH_FLAGS 0x0					//	|
#define MBH_CHECKSUM -(MBH_MAGIC + MBH_FLAGS)		//	|
							//	|
.align 4						//	|
.int MBH_MAGIC						//	|
.int MBH_FLAGS						//	|
.int MBH_CHECKSUM					//	|


.section .text						//Text Section

.global start						//Globale Funktionen (auch von Funktionen außerhalb dieser Datei aufrufbar)

.extern ctors
.extern init

start:							//Hier ist der Start des Kernel
	mov $stack, %esp				//Stack einrichten

	push %eax
	push %ebx

	call ctors					//Konstruktoren aufrufen
	call init					//C++ Funktion aufrufen, ab jetzt geht es mit C++ weiter

halt:							//Funktion zum Anhalten des Systems
	cli
	hlt
	jmp halt

.section .bss						//Bss Section
.align 4096
.space 8192						//Stack einrichten
stack:
