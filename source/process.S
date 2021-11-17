.type get_eip, @function
.globl get_eip

get_eip:
	popl %eax	# load eip -> eax
	jmp *%eax
