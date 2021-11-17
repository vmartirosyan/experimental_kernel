.global idt_flush 

idt_flush:
	lidt idt_ptr
	ret
