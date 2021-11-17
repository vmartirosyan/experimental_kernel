#ifndef GDT_ENTRY_H
#define GDT_ENTRY_H

#include "descriptor.h"
#include "tss_entry.h"

class GdtDescriptor: public Descriptor
{
public:
	GdtDescriptor(unsigned long limit, unsigned long base, unsigned char access, unsigned char granularity);
	
	// set basic descriptor, use for segment descriptors, ...
	void set(unsigned long limit, unsigned long base, unsigned char access, unsigned char granularity);
	
	// set Task State Segment (TSS) Descriptor, Intel 7.2.2
	void setTss(TssEntry* tssEntry, unsigned short ss0, unsigned int esp0);

}__attribute__((packed));

struct gdt_pointer
{
    unsigned short limit_;
    unsigned int base_;
}__attribute__((packed));

extern "C" void gdt_install();

#endif	// GDT_ENTRY_H
