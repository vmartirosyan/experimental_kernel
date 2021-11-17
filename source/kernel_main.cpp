#include "screen.h"
#include "pic_timer.h"
#include "paging_management.h"
#include "memory_management.h"
#include "kernel_heap.h"
#include "vfs/fs.h"
#include "vfs/initrd_fs.h"
#include "process.h"
#include "multiboot.h"
#include "tss_entry.h"
#include "system_calls.h"
#include "user_space_system_calls.h"


unsigned int initialEsp;
Screen screen;
PicTimer picTimer;
PagingManagement pmmu;
KernelHeap kernelHeap;
Vfs vfs;
Sheduler sheduler;
extern SystemCallManagement systemCallManagement;

//InitrdFs initrdFs;
extern "C" void switch_to_user_mode();
extern "C" void switch_to_kernel_mode();

extern "C" void tss_flush();

void printFlags();

extern unsigned int kernel_end;
extern unsigned int placement_address;

extern "C" unsigned int user_print_screen()
{
}

extern "C" int kernel_main(struct multiboot* mb_header, unsigned int magic, unsigned int stack_ptr)
{
	screen.printString("Kernel start ...\n");
	
	initialEsp = stack_ptr;
	placement_address = reinterpret_cast<unsigned int>(&kernel_end);
	screen.clean();

    if ( magic != 0x2BADB002 )
    {
		screen.printString("Invalid magic number\0");
		for(;;);
	}

	// Find the location of our initial ramdisk.
	if (! mb_header->mods_count > 0)
	{
		screen.printString("There are no loaded modules ... \n");
	}
	
    picTimer.init();
	
	//http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format
	unsigned int initrdLocation = *((unsigned int*)mb_header->mods_addr);	
	unsigned int initrdEnd = *(unsigned int*)(mb_header->mods_addr+sizeof(unsigned int));
	
	placement_address = initrdEnd;
	
	screen.printString("GDT and IDT tables are initialised\n");
	
	pmmu.initialisePaging();
	systemCallManagement.init();
	/*
	char * a = (char *)kernelHeap.allocate(8, true);
	char * b = (char *)kernelHeap.allocate(8, true);
	memcpy(a, const_cast<char*>("abcdefg\0"), 8);
	screen.printString(a);
	screen.printEndl();
	memcpy(b, const_cast<char*>("gfedcba\0"), 8);
	screen.printString(a);
	screen.printEndl();
	screen.printString(b);
	screen.printEndl();	*/
	screen.printString("Paging is enabled and kernel heap is initialised\n");
	
	/*// Test page fault
	unsigned int *ptr = (unsigned int*)0xFFFFFFFF;
	unsigned int do_page_fault = *ptr;
	*/
	
	
	//screen.clean();
	sheduler.init();
	screen.printString("Sheduler is initialised\n");		

	InitrdFs:: init(initrdLocation); // initrdFs.root_ initialised
	
	// Read initrd in child process;

	int pid = sheduler.fork();
	screen.printString("\nfork: \n");
	
	__asm__("sti");
	screen.printString("process pid: ");
	screen.printHex(pid);
	screen.printEndl();
	screen.printString("*************************\n");
	sheduler.switchTask();
	if (pid == 0)
	{
		//child
		// vfs
		int i = 0;
		dirent* node = 0;
		screen.printString("Read contents of initrd\n");
		while ((node = vfs.readdir(InitrdFs:: root_, i)) != 0)
		{
			screen.printString("Found file: ");
			screen.printString(node->name);
			//screen.printString(" ");
			//screen.printHex(i);
			screen.printEndl();
					
			FileNode *fsnode = vfs.finddir(InitrdFs:: root_, node->name);
			//todo
			//fsnode->inode_ = i;
			if ((fsnode->flags_ & 0x7) == FileNode:: FS_DIRECTORY)
			{
				screen.printString(" - directory -> \n");
			}
			else
			{
				unsigned char buf[256];
				unsigned int sz = vfs.read(fsnode, 0, 256, buf);
				int j;
				for (j = 0; j < sz; j++)
					screen.printChar(buf[j]);
				screen.printEndl();
			}			
			i++;
		}
		
		screen.printString("Switch to user space (switch to ring 3)\n");
		systemCallManagement.switchToUserMode();
		syscall_print_string("Hello from userspace!\n");
		
	}
	else
	{
		//parent
		//screen.printString("should wait for child\n");
	}
	
   // screen.printString("... kernel return");
   // __asm__("cli");
    for(;;);

	systemCallManagement.init();
	
	systemCallManagement.switchToUserMode();
	
	// We are in user mode; use system calls :)

	syscall_print_string("Hello from userspace!\n");
    for(;;);
    return 0;
}

void printFlags()
{
	unsigned int eflags = 0;
	screen.setTextColor(COLOR_BLACK, COLOR_LIGHT_BLUE);
	__asm__("	\
	pushfl;	\
	popl %%eax;	\
	movl %%eax, %0;	\
	" : "=r" (eflags));
	screen.printString("eflags = ");
	screen.printBinary(eflags);
}
