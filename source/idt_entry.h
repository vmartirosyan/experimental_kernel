#ifndef IDT_ENTRY_H
#define IDT_ENTRY_H

#include "descriptor.h"

class IdtDescriptor: public Descriptor
{
public:
	IdtDescriptor();
	IdtDescriptor(unsigned long offset, unsigned short selector, unsigned char flags);
	
	void setIdtDescriptor(unsigned long offset, unsigned short selector, unsigned char flags);

} __attribute__((packed));

struct idt_pointer
{
	unsigned short limit_;
    unsigned int base_;
}__attribute__((packed));

extern "C" 
{
	extern void idt_flush();
}

extern "C" void idt_install();

#endif // IDT_ENTRY_H
