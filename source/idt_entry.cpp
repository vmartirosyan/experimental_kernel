#include "idt_entry.h"
#include "isr.h"
#include "screen.h"
#include "pic_timer.h"

extern Screen screen;
extern PicTimer picTimer;

IdtDescriptor:: IdtDescriptor()
{
	limitLow_		= 0;
	baseLow_		= 0;
	baseMiddle_		= 0;
	access_			= 0;
	granularity_	= 0;
	baseHigh_		= 0;
}
IdtDescriptor:: IdtDescriptor(unsigned long offset, unsigned short selector, unsigned char flags)
{
	limitLow_				= offset & 0xFFFF;
	baseLow_				= selector;
	baseMiddle_				= 0;
	access_					= flags;
	unsigned short base_h	= (offset >> 16) & 0xFFFF;
	granularity_			= base_h & 0xFF;
	baseHigh_				= (base_h >> 8) & 0xFF;
}

void IdtDescriptor::setIdtDescriptor(unsigned long offset, unsigned short selector, unsigned char flags)
{
	limitLow_				= offset & 0xFFFF;
	baseLow_				= selector;
	baseMiddle_				= 0;
	access_					= flags;
	unsigned short base_h	=	(offset >> 16) & 0xFFFF;
	granularity_			= base_h & 0xFF;
	baseHigh_				= (base_h >> 8) & 0xFF;
}

idt_pointer idt_ptr;
const unsigned int IDT_SIZE = 256;
IdtDescriptor idt[IDT_SIZE];

extern "C" void idt_install()
{
    idt_ptr.limit_ = (sizeof (IdtDescriptor) * 256) - 1;
    idt_ptr.base_ = (unsigned int)&idt;

	// Remap the irq table.
	__asm__(" \
			movb $0x11, %al; \
			outb $0x20;		\
			");
	__asm__(" \
			movb $0x11, %al; \
			outb $0xA0;		\
			");
	__asm__(" \
			movb $0x20, %al; \
			outb $0x21;		\
			");
	__asm__(" \
			movb $0x28, %al; \
			outb $0xA1;		\
			");
	__asm__(" \
			movb $0x04, %al; \
			outb $0x21;		\
			");
	__asm__(" \
			movb $0x02, %al; \
			outb $0xA1;		\
			");
	__asm__(" \
			movb $0x01, %al; \
			outb $0x21;		\
			");
	__asm__(" \
			movb $0x01, %al; \
			outb $0xA1;		\
			");
	__asm__(" \
			movb $0x0, %al; \
			outb $0x21;		\
			");
	__asm__(" \
			movb $0x0, %al; \
			outb $0xA1;		\
			");
  
	// ISRS install, CPU exceptions
	idt[0].setIdtDescriptor((unsigned)isr0, 0x08, 0x8E);
	idt[1].setIdtDescriptor((unsigned)isr1, 0x08, 0x8E);
	idt[2].setIdtDescriptor((unsigned)isr2, 0x08, 0x8E);
	idt[3].setIdtDescriptor((unsigned)isr3, 0x08, 0x8E);
	idt[4].setIdtDescriptor((unsigned)isr4, 0x08, 0x8E);
	idt[5].setIdtDescriptor((unsigned)isr5, 0x08, 0x8E);
    idt[6].setIdtDescriptor((unsigned)isr6, 0x08, 0x8E);
    idt[7].setIdtDescriptor((unsigned)isr7, 0x08, 0x8E);
    idt[8].setIdtDescriptor((unsigned)isr8, 0x08, 0x8E);
    idt[9].setIdtDescriptor((unsigned)isr9, 0x08, 0x8E);
    idt[10].setIdtDescriptor((unsigned)isr10, 0x08, 0x8E);
    idt[11].setIdtDescriptor((unsigned)isr11, 0x08, 0x8E);
    idt[12].setIdtDescriptor((unsigned)isr12, 0x08, 0x8E);
    idt[13].setIdtDescriptor((unsigned)isr13, 0x08, 0x8E);
    idt[14].setIdtDescriptor((unsigned)isr14, 0x08, 0x8E);
    idt[15].setIdtDescriptor((unsigned)isr15, 0x08, 0x8E);
    idt[16].setIdtDescriptor((unsigned)isr16, 0x08, 0x8E);
    idt[17].setIdtDescriptor((unsigned)isr17, 0x08, 0x8E);
    idt[18].setIdtDescriptor((unsigned)isr18, 0x08, 0x8E);
    idt[19].setIdtDescriptor((unsigned)isr19, 0x08, 0x8E);
    idt[20].setIdtDescriptor((unsigned)isr20, 0x08, 0x8E);
    idt[21].setIdtDescriptor((unsigned)isr21, 0x08, 0x8E);
    idt[22].setIdtDescriptor((unsigned)isr22, 0x08, 0x8E);
    idt[23].setIdtDescriptor((unsigned)isr23, 0x08, 0x8E);
    idt[24].setIdtDescriptor((unsigned)isr24, 0x08, 0x8E);
    idt[25].setIdtDescriptor((unsigned)isr25, 0x08, 0x8E);
    idt[26].setIdtDescriptor((unsigned)isr26, 0x08, 0x8E);
    idt[27].setIdtDescriptor((unsigned)isr27, 0x08, 0x8E);
    idt[28].setIdtDescriptor((unsigned)isr28, 0x08, 0x8E);
    idt[29].setIdtDescriptor((unsigned)isr29, 0x08, 0x8E);
    idt[30].setIdtDescriptor((unsigned)isr30, 0x08, 0x8E);
    idt[31].setIdtDescriptor((unsigned)isr31, 0x08, 0x8E);

	idt[32].setIdtDescriptor((unsigned)irq0, 0x08, 0x8E); // timer
    idt[33].setIdtDescriptor((unsigned)irq1, 0x08, 0x8E);
    idt[34].setIdtDescriptor((unsigned)irq2, 0x08, 0x8E);
    idt[35].setIdtDescriptor((unsigned)irq3, 0x08, 0x8E);
    idt[36].setIdtDescriptor((unsigned)irq4, 0x08, 0x8E);
    idt[37].setIdtDescriptor((unsigned)irq5, 0x08, 0x8E);
    idt[38].setIdtDescriptor((unsigned)irq6, 0x08, 0x8E);
    idt[39].setIdtDescriptor((unsigned)irq7, 0x08, 0x8E);
    idt[40].setIdtDescriptor((unsigned)irq8, 0x08, 0x8E);
    idt[41].setIdtDescriptor((unsigned)irq9, 0x08, 0x8E);
    idt[42].setIdtDescriptor((unsigned)irq10, 0x08, 0x8E);
    idt[43].setIdtDescriptor((unsigned)irq11, 0x08, 0x8E);
    idt[44].setIdtDescriptor((unsigned)irq12, 0x08, 0x8E);
    idt[45].setIdtDescriptor((unsigned)irq13, 0x08, 0x8E);
    idt[46].setIdtDescriptor((unsigned)irq14, 0x08, 0x8E);
    idt[47].setIdtDescriptor((unsigned)irq15, 0x08, 0x8E);
    
    idt[128].setIdtDescriptor((unsigned)isr128, 0x08, 0x8E | 0x60); // or 0x60 - set DPL - 3, to use int instruction in user space
}


