#include "tss_entry.h"

// single tss entry, uses for switch between user and kernel space;
TssEntry tssEntry;

void TssEntry:: setKernelStack(unsigned int kernelEsp)
{
	esp0 = kernelEsp;
}
