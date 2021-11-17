#include "isr.h"
#include "screen.h"

extern Screen screen;

const char* exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint Exception",
	"Into Detected Overflow Exception",
	"Out of Bounds Exception",
	"Invalid Opcode Exception",
	"No Coprocessor Exception",
	"Double Fault Exception",
	"Coprocessor Segment Overrun Exception",
	"Bad TSS Exception",
	"Segment Not Present Exception",
	"Stack Fault Exception",
	"General Protection Fault Exception",
	"Page Fault Exception",
	"Unknown Interrupt Exception",
	"Coprocessor Fault Exception",
	"Alignment Check Exception (486+)",
	"Machine Check Exception (Pentium/586+)",
	"Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "System Call Exception"
};

//
interrupt_handler isr_handlers[129];
void register_isr_handler(int num, interrupt_handler handler)
{
	isr_handlers[num] = handler;
}

// isr handler
extern "C" void fault_handler(registers r)
{
	if (r.int_number < 33 || r.int_number == 128)
	{
		if(isr_handlers[r.int_number])
		{
			isr_handlers[r.int_number](r);
		}
		else
		{
			screen.setTextColor(COLOR_BLACK, COLOR_LIGHT_RED);
			screen.printEndl();
			screen.printString("EXCEPTION  \0");
			screen.printInt(r.int_number);
			screen.printEndl();
			screen.printString(exception_messages[r.int_number]);
			screen.printEndl();
		}
		for(;;); 
	}
}

//
interrupt_handler irq_handlers[16];
void register_irq_handler(int num, interrupt_handler handler)
{
	irq_handlers[num] = handler;
}

extern "C" void irq_handler(registers r)
{
    // Send an EOI (end of interrupt) signal to the Programmable Interrupt Controller (PIC)
    // If this interrupt involved the slave.
    if (r.int_number >= 40)
    {
        // Send reset signal to slave.
        __asm__(" \
			movb $0xA0, %al; \
			outb $0x20;		\
			");
    }
    // Send reset signal to master
    __asm__(" \
		movb $0x20, %al; \
		outb $0x20;		\
		");

	if(r.int_number == 32 && irq_handlers[r.int_number - 32] != 0) // for other irq_handler is not set  
	{
		/*
		screen.print_string((unsigned char *)"timer");
		screen.printString(" - ");
		screen.printInt(r.int_number);
		screen.printString(" - ");
		*/
		irq_handlers[r.int_number - 32](r);
	}
	else
	{
		//screen.printString((unsigned char *)"not timer - ");
		//screen.printInt(r.int_number);		
	}
	screen.setTextColor(COLOR_BLACK, COLOR_WHITE);
}
