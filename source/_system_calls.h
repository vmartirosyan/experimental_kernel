#ifndef SYSTEM_CALLS_H
#define SYSTEM_CALLS_H

#include "registers.h"
#include "screen.h"

const int SYSTEM_CALLS_NUM = 1;
extern Screen screen;

void print_screen()
{
	screen.print_string((unsigned char *)"print screen system call");
}

void * system_calls[SYSTEM_CALLS_NUM] = { (void *)print_screen };
extern "C" void init_system_calls();
void system_call_handler(registers);

#endif
