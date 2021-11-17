#include "process.h"
#include "paging_management.h"
#include "memory_management.h"

extern PagingManagement pmmu;
extern unsigned int initialEsp;

extern Sheduler sheduler;

extern "C" unsigned int get_eip();

//unsigned int switchTimes = 0;

// Process
Process:: Process()
{
	pid_	= 0;
	esp_	= 0;
	ebp_	= 0;
	eip_	= 0;
	pageDirectory_	= 0;	
}

Process:: Process(unsigned int& pidCounter)
{
	pid_	= pidCounter++;
	esp_	= 0;
	ebp_	= 0;
	eip_	= 0;
	pageDirectory_	= 0;
}

Process:: Process(unsigned int& pidCounter, PageDirectory* pageDirectory)
{
	pid_	= pidCounter++;
	esp_	= 0;
	ebp_	= 0;
	eip_	= 0;
	pageDirectory_	= pageDirectory;
}

//
ProcessSLinkedList:: ProcessSLinkedList(): SLinkedList()
{
	currentProcess_ = 0;
	currentProcess_->next_ = 0;
}

bool ProcessSLinkedList:: pushBackCurrent(Process proc)
{
	if(!pushBack(proc))
		return false;
	currentProcess_ = last_;
	return true;
}

Process* ProcessSLinkedList:: getCurrentProcessPtr()
{
	return &currentProcess_->data_;
}

//
Sheduler:: Sheduler()
{
	
}

Sheduler:: Sheduler(int a)
{	
	__asm__("cli");
	pidCounter_ = 1;
	
	changeStack((void*)0xE0000000, 0x2000);
	processList_.pushBackCurrent(Process(pidCounter_, pmmu.currentPageDirectory_));

	__asm__("sti");
}

Sheduler:: ~Sheduler()
{
	//delete List
}

void Sheduler:: init()
{
	__asm__("cli");
	pidCounter_ = 1;
	
	changeStack((void*)0xE0000000, 0x2000);

	processList_.pushBackCurrent(Process(pidCounter_, pmmu.currentPageDirectory_));
	__asm__("sti");
}

unsigned int Sheduler:: getEip()
{
	unsigned int eip = 0;
	__asm__("call 1f \n"
			"1: pop %0" : "=r"(eip));
	return eip; 
}

void Sheduler:: switchTask()
{			
	if (!processList_.currentProcess_)
		return;
	
	//switchTimes++;
	
	//screen.printString("task switch");	
	unsigned int esp = 0;
	__asm__("movl %%esp, %0" : "=r"(esp));
	unsigned int ebp = 0;
	__asm__("movl %%ebp, %0" : "=r"(ebp));
	
	unsigned int eip = get_eip();
	/*screen.printString("eip - ");
	screen.printHex(eip);
	screen.printEndl();*/

	if(eip == DUMMY_EIP)
	{
		// there was a task switch
		//screen.printString("dummy eip :D \n");
		return;
	}
	else
	{
		//if (pidCounter_ > 1 && processList_.currentProcess_ != 0 && processList_.currentProcess_->next_ != 0 && processList_.currentProcess_->data_.pageDirectory_ != 0)
		//{
			//screen.printString("not dummy eip\n");
			// TODO processList_.currentProcess_->set();
			processList_.currentProcess_->data_.eip_ = eip;
			processList_.currentProcess_->data_.esp_ = esp;
			processList_.currentProcess_->data_.ebp_ = ebp;
			// switch tasks	
			processList_.currentProcess_ = processList_.currentProcess_->next_;
			if (!processList_.currentProcess_)
			{
				processList_.currentProcess_ = processList_.first_;
			}
			eip = processList_.currentProcess_->data_.eip_;
			esp = processList_.currentProcess_->data_.esp_;
			ebp = processList_.currentProcess_->data_.ebp_;
			
			pmmu.currentPageDirectory_ = processList_.currentProcess_->data_.pageDirectory_;
			
			// put movl %2, %%ecx at first and - eip goes in ebp, fail!
			// eax - return value for function/method
			__asm__("cli;	\
					movl %0, %%esp;	\
					movl %1, %%ebp;	\
					movl %2, %%ecx;	\
					movl %3, %%cr3;	\
					movl %4, %%eax;	\
					sti;	\
					jmp *%%ecx;":: "r"(esp), "r"(ebp), "r"(eip), "r"(processList_.currentProcess_->data_.pageDirectory_->physicalAddress), "r"(DUMMY_EIP));
	}
}

int Sheduler:: fork()
{
	__asm__("cli");
	Process* parentProc = processList_.getCurrentProcessPtr();
	PageDirectory* pDir = pmmu.clonePageDirectory(pmmu.currentPageDirectory_);
	/*screen.printString("pDir->pageTables[0x31C] - ");
	screen.printHex(pDir->pageTablesPhysical[0x31C]);
	screen.printEndl();
	screen.printHex(reinterpret_cast<unsigned int>(&processList_));
	screen.printEndl();*/
	Process childProc(pidCounter_, pDir);
	
	unsigned int eip = get_eip(); // this eip goes for child process start
		
	// HERE IS THE PROBLEM OF PAGE FAULT!!!
	/*screen.printHex(reinterpret_cast<unsigned int>(&processList_));
	
	screen.printHex(*(reinterpret_cast<unsigned int*>(&sheduler.processList_))); // this is OK
	screen.printHex(*(reinterpret_cast<unsigned int*>(&processList_))); // Fault
	screen.printString("\n - ");
	screen.printEndl();*/
	
	if (processList_.getCurrentProcessPtr() == parentProc)
	{
		//screen.printString("we are parent\n");
		unsigned int esp = 0;
		__asm__("movl %%esp, %0" : "=r"(esp));
		unsigned int ebp = 0;
		__asm__("movl %%ebp, %0" : "=r"(ebp));
		childProc.esp_	= esp;
		childProc.ebp_	= ebp;
		childProc.eip_	= eip;

/*		screen.printString("process list addr - \n");
		screen.printHex(reinterpret_cast<unsigned int>(&processList_));
		screen.printEndl();*/
		
		processList_.pushBack(childProc);
		
		__asm__("sti");
		return childProc.pid_;
	}
	else
	{
		//screen.printString("we are child\n");
		__asm__("sti");
		return 0;
	}
}

int Sheduler:: getPid()
{
	return 0;
}

void Sheduler:: changeStack(void* newStartAddr, unsigned int newSize)
{
	// stack grows up
	for (unsigned int i = reinterpret_cast<unsigned int>(newStartAddr); i >= reinterpret_cast<unsigned int>(newStartAddr) - newSize; i -= Page:: SIZE)
	{
		pmmu.allocFrame(pmmu.getPage(i, true, pmmu.currentPageDirectory_), false, true);	// user mode, writable
	}
	
	// flush TLB
	unsigned int pdAddr;
	__asm__("mov %%cr3, %0" : "=r"(pdAddr));	
	__asm__("mov %0, %%cr3" :: "r"(pdAddr));

	unsigned int oldEsp;
	__asm__("movl %%esp, %0": "=r"(oldEsp));
	unsigned int oldEbp;
	__asm__("movl %%ebp, %0": "=r"(oldEbp));
	
	unsigned int offset = reinterpret_cast<unsigned int>(newStartAddr) - initialEsp;
	
	unsigned int newEsp = reinterpret_cast<unsigned int>(newStartAddr) + (oldEsp - initialEsp);//oldEsp + offset;
	unsigned int newEbp = reinterpret_cast<unsigned int>(newStartAddr) + (oldEbp - initialEsp);//oldEbp + offset;
	
	// copy stack entries, which were added after kernel start
	memcpy(reinterpret_cast<void*>(newEsp), reinterpret_cast<void*>(oldEsp), initialEsp - oldEsp);  
	// copy stack entries, which were added before kernel start
	for (unsigned int i = reinterpret_cast<unsigned int>(newStartAddr); i > reinterpret_cast<unsigned int>(newStartAddr) - newSize; i -= sizeof(int))
	{
		unsigned int tmp = *(reinterpret_cast<unsigned int*>(i));
		if ((oldEsp < tmp) && (tmp < initialEsp))
		{
			tmp = tmp + offset;
			unsigned int* tmp2 = reinterpret_cast<unsigned int*>(i);
			*tmp2 = tmp;
		}
	}
	
	//load esp, ebp
	__asm__("movl %0, %%esp" : : "r" (newEsp));
	__asm__("movl %0, %%ebp" : : "r" (newEbp));
}

const Process& Sheduler:: getCurrentProcess()
{
	return *(processList_.getCurrentProcessPtr());
}
