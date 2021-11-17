#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include "user_space_system_calls.h"
#include "registers.h"

int monitor_write(const char *);
int monitor_write_hex(const char *);
int monitor_write_dec(const char *);

class SystemCallManagement
{
public:
	SystemCallManagement();
	void init();
	void switchToUserMode();
	
	static const int NUMBER = 3;
	static void* systemCalls_[NUMBER];
private:
};

extern "C" void system_call_handler(registers regs);

#endif	// SYSTEM_CALL_H
