.global loader                 

# GRUB docs
# MULTIBOOT HEADER
.set ALIGN,    1<<0             
.set MEMINFO,  1<<1             
.set FLAGS,    ALIGN | MEMINFO  
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS) 

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


.set STACKSIZE, 0x4000          
.comm stack, STACKSIZE, 32      

loader:
   mov  $(stack + STACKSIZE), %esp
   
   push %esp		# stack pointer
   push %eax		# magic number             
   push %ebx		# multiboot header
      
   mov  $start_ctors, %ebx
   jmp  2f
 
1:
   call *(%ebx)
   add  $4, %ebx
 
2:
   cmp  $end_ctors, %ebx
   jb   1b
    
 # enter protected mode, install GDT, install IDT #
	cli
	call gdt_install	
	call idt_install
	
 	lgdt gdt_ptr
	lidt idt_ptr
	
	movl %CR0, %eax
	or $1, %AL
	movl %eax, %CR0
	
	ljmp $0x08, $flush   
flush:
	mov $0x10,%ax      			
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    mov %ax,%ss
 ###
	call tss_flush	# set TR (task register)
	call kernel_main

	mov  $end_dtors, %ebx               # call the destructors
    jmp  4f
3:
    sub  $4, %ebx
    call *(%ebx)
4:
    cmp  $start_dtors, %ebx
    ja   3b

    hlt
