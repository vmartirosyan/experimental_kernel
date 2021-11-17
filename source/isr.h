#ifndef ISR_H
#define ISR_H
#include "registers.h"

/* Interrupt service routine (ISR),
 * defined in interrupt.S
 */ 
extern "C" void isr0(); // 0  	Division By Zero Exception
extern "C" void isr1(); // 1 	Debug Exception 
extern "C" void isr2(); // 2 	Non Maskable Interrupt Exception
extern "C" void isr3(); // 3 	Breakpoint Exception
extern "C" void isr4(); // 4 	Into Detected Overflow Exception 	
extern "C" void isr5(); // 5 	Out of Bounds Exception
extern "C" void isr6(); // 6 	Invalid Opcode Exception
extern "C" void isr7(); // 7 	No Coprocessor Exception
extern "C" void isr8(); // 8 	Double Fault Exception
extern "C" void isr9(); // 9 	Coprocessor Segment Overrun Exception
extern "C" void isr10(); // 10 	Bad TSS Exception
extern "C" void isr11(); // 11 	Segment Not Present Exception
extern "C" void isr12(); // 12 	Stack Fault Exception  
extern "C" void isr13(); // 13 	General Protection Fault Exception 
extern "C" void isr14(); // 14 	Page Fault Exception
extern "C" void isr15(); // 15 	Unknown Interrupt Exception 
extern "C" void isr16(); // 16 	Coprocessor Fault Exception 
extern "C" void isr17(); // 17 	Alignment Check Exception (486+) 
extern "C" void isr18(); // 18 	Machine Check Exception (Pentium/586+) 

// 19 - 31  	Reserved Exceptions 
extern "C" void isr19(); 
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

extern "C" void isr128(); // 0x80 system call interrupt

/* Interrupt request (IRQ)
 * defined in interrupt.S
 */
extern "C" void irq0(); // timer, 32
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

typedef void (*interrupt_handler)(registers);

void register_isr_handler(int num, interrupt_handler handler);
void register_irq_handler(int num, interrupt_handler handler);

extern "C" void irq_handler(registers);
#endif
