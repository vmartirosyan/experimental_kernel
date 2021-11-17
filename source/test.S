.global switch_to_user_mode
.global switch_to_kernel_mode

switch_to_user_mode:
	ljmp $0x1B, $flush   
flush:
	mov $0x23,%ax      			
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    mov %ax,%ss
