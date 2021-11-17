#ifndef TSS_ENTRY_H
#define TSS_ENTRY_H

// Task State Segment

extern "C" void tss_flush();

// Intel 7.2.1
class TssEntry
{
public:
	unsigned int previousTasklink;   // Previous task link field
	//Privilege level-0, -1, and -2 stack pointer fields
	unsigned int esp0;
	unsigned int ss0;
	unsigned int esp1;
	unsigned int ss1;
	unsigned int esp2;
	unsigned int ss2;
	//
	unsigned int cr3;
	unsigned int eip;
	unsigned int eflags;
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
	// Segment selectors stored in the ES, CS, SS, DS, FS, and GS registers prior to the task switch.
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
	unsigned int fs;
	unsigned int gs;
	//
	unsigned int ldt;		// ldt segment selector
	unsigned short trap;
	unsigned short iomapBaseAddr;

	void setKernelStack(unsigned kernelEsp);
private:
}__attribute__((packed));


/*
 * Intel 7.2.2
 * TSS descriptors may only be placed in the GDT
*/
/*
class TssDescriptor: public GdtDescriptor
{
public:
	TssDescriptor();
	TssDescriptor(int index, unsigned short ss0, unsigned int esp0);
	void set(int index, unsigned short ss0, unsigned int esp0);
};*/
#endif	// TSS_ENTRY_H
