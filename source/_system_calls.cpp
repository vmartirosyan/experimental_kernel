#include "system_calls.h"
#include "IdtEntry.h"

extern IdtDescriptor idt[];
extern "C" void SystemCall();

extern "C" void init_system_calls()
{
		idt[32].setIdtDescriptor((unsigned)SystemCall, 0x08, 0x8E);
}

void system_call_handler(registers reg)
{
	void * syscall_handler = system_calls[reg.eax];
}


