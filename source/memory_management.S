.global copy_page_physical
 
copy_page_physical:
	pushl %ebx
	pushfl
	
	cli
	
	movl 12(%esp), %ebx		#load source address from stack
	movl 16(%esp), %ecx		#load dest address from stack
	
	# disable paging
	movl %cr0, %eax
	andl $0x7FFFFFFF, %eax
	movl %eax, %cr0
	
	movl $1024, %edx
loop:
	movl (%ebx), %eax
	movl %eax, (%ecx)
	addl $4, %ebx
	addl $4, %ecx
	decl %edx
	jnz loop

	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0
	
	popfl
	popl %ebx
	ret
