#include "gdt_entry.h"
#include "screen.h"
#include "memory_management.h"
#include "tss_entry.h"

extern TssEntry tssEntry;

GdtDescriptor:: GdtDescriptor(unsigned long base = 0, unsigned long limit = 0, unsigned char access = 0, unsigned char granularity = 0): Descriptor(base, limit, access, granularity)
{
	
}

void GdtDescriptor:: set(unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity)
{
	limitLow_		= (limit & 0xFFFF);     
    baseLow_		= (base & 0xFFFF);
    baseMiddle_		= (base >> 16) & 0xFF;
    access_			= access;
    
    granularity_	= (limit >> 16) & 0x0F;
    granularity_ |= (granularity & 0xF0);
    
    baseHigh_		= (base >> 24) & 0xFF;
}

void GdtDescriptor:: setTss(TssEntry* tssEntry, unsigned short ss0, unsigned int esp0)
{
	unsigned int base = reinterpret_cast<unsigned int>(tssEntry);
	unsigned int limit = base + sizeof(*tssEntry);
	
	set(base, limit, 0xE9, 0x00);
	
	memset(tssEntry, 0, sizeof(TssEntry));
	
	tssEntry->ss0	= ss0;
	tssEntry->esp0	= esp0;
	
	//RPL - 11b = 3;
	tssEntry->cs	= 0x0B;
	tssEntry->ss	= 0x13;
	tssEntry->ds	= 0x13;
	tssEntry->es	= 0x13;
	tssEntry->fs	= 0x13;
	tssEntry->gs	= 0x13;
}

gdt_pointer gdt_ptr;

// GDT table
const int GDT_SIZE  = 6;
GdtDescriptor gdt[GDT_SIZE];

extern Screen screen;

extern "C" void gdt_install()
{
	screen.clean();
	int i;

    gdt_ptr.limit_	= (sizeof(GdtDescriptor) * GDT_SIZE) - 1;
    gdt_ptr.base_	= (unsigned int)&gdt;

    gdt[0].set(0, 0, 0, 0);
    gdt[1].set(0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment, DPL 00
    gdt[2].set(0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment, DPL 00  
    gdt[3].set(0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment, DPL 11
    gdt[4].set(0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment, DPL 11
    gdt[5].setTss(&tssEntry, 0x10, 0x0);
}
