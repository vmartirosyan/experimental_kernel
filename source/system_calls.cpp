#include "system_calls.h"
#include "isr.h"
#include "screen.h"
#include "tss_entry.h"
#include "process.h"
#include "paging_management.h"

extern Screen screen;
extern TssEntry tssEntry;
extern Sheduler sheduler;
extern PagingManagement pmmu;

SystemCallManagement systemCallManagement;
//

int monitor_write(const char * str)
{
	screen.printString(str);
	return 0;
}

int monitor_write_hex(const char * str)
{
	screen.printString(str);
	return 0;
}

int monitor_write_dec(const char * str)
{
	screen.printString(str);
	return 0;
}

//
void* SystemCallManagement:: systemCalls_[NUMBER] = 
{
	reinterpret_cast<void*>(monitor_write), 
	reinterpret_cast<void*>(monitor_write_hex), 
	reinterpret_cast<void*>(monitor_write_dec)
};

SystemCallManagement:: SystemCallManagement()
{

}

void SystemCallManagement:: init()
{
	register_isr_handler(128, system_call_handler);
}

void SystemCallManagement:: switchToUserMode()
{
	//tssEntry.setKernelStack(sheduler.getCurrentProcess().esp_ + 2048); // 2048 - leave this space for stack
	tssEntry.setKernelStack(tssEntry.esp0 + 2048); // 2048 - leave this space for stack
	screen.printString("tssEntry.esp0 - ");
	screen.printHex(tssEntry.esp0);
	screen.printEndl();
	screen.printString("tssEntry.ss0 - ");
	screen.printHex(tssEntry.	ss0);
	screen.printEndl();
	/* iret instruction expect the following values in stack:  | SS | ESP | EFLAGS | CS | EIP | (stack grows ->)
	 * at label 1 we should be in USER MODE (due to iret Intel docs, Executing a task 7.1.3 ), here we should enable interrupts setting bit  EFLAGS value (as we are noy in kernel mode)
	 * */
	__asm__("  \
	cli; \
	mov $0x23, %ax; \
	mov %ax, %ds; \
	mov %ax, %es; \
	mov %ax, %fs; \
	mov %ax, %gs; \
	mov %esp, %eax; \
	pushl $0x23; \
	pushl %eax; \
	pushf; \
	pop %eax;	\
	orl $0x200, %eax;	\
	pushl %eax;	\
	pushl $0x1B; \
	\
	push $1f; \
	iret; \
	1: \
	"); 

}

extern "C" void system_call_handler(registers regs)
{
	//screen.printString("Hello from user mode!\n");
	//return;
	
	if(regs.eax >= SystemCallManagement:: NUMBER) // true - system call with this number is not realised in kernel yet
		return;
	
	void* location = systemCallManagement.systemCalls_[regs.eax];
	
	int ret;
	__asm__(" \
	push %1; \
	push %2; \
	push %3; \
	push %4; \
	push %5; \
	call *%6; \
	pop %%ebx; \
	pop %%ebx; \
	pop %%ebx; \
	pop %%ebx; \
	pop %%ebx; \
	" : "=a" (ret) : "r" (regs.edi), "r" (regs.esi), "r" (regs.edx), "r" (regs.ecx), "r" (regs.ebx), "r" (location));
	regs.eax = ret;
	
	for(;;);
}
