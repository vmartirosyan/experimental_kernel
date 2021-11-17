#include "isr.h"
#include "memory_management.h"
#include "paging_management.h"
#include "kernel_heap.h"

extern unsigned int placement_address;
extern unsigned int kernel_end;
extern "C" void copy_page_physical(unsigned int, unsigned int);
extern KernelHeap kernelHeap;

// Bitset
void Bitset:: set(unsigned int offset)
{
	bitset_ |= (0x1 << offset);
}

void Bitset:: clear(unsigned int offset)
{
   bitset_ &= ~(0x1 << offset);
}

bool Bitset:: test(unsigned int offset)
{
   return (bitset_ & (0x1 << offset));
}

unsigned int Bitset:: getBitset()
{
	return bitset_;
}

// MemoryManagemntUnit
PagingManagement:: PagingManagement()
{
	// TODO:
	enabled_ = false;
}

void PagingManagement:: initialisePaging()
{
	__asm__("cli");
	// Main memory size = 16 MB
	unsigned int memSize = 0x1000000; // 2 ^ 24 = 16 MB

	frameNumber_ = memSize / 0x1000; // 2 ^ 12 - frame size
	frames_ = (Bitset*)kmalloc(frameNumber_/32 * sizeof(Bitset), 0, 0); // 1 bit for frame to mark frame presence/absence
	memset(frames_, 0, frameNumber_/32 * sizeof(Bitset)); // set all frames are free
	
	// init Page Directory
	unsigned int phys = 0;
	pageDirectory_ = reinterpret_cast<PageDirectory*>(kmalloc(sizeof(PageDirectory), 1, &phys));
	memset(pageDirectory_, 0, sizeof(PageDirectory));
	
	// init Page Tables, empty ones, 0 ptrs
	
	/*pageDirectory_->pageTables = reinterpret_cast<PageTable**>(kmalloc(PageDirectory::SIZE * sizeof(PageTable*),0, 0));
	memset(pageDirectory_->pageTables, 0, PageTable::SIZE * sizeof(PageTable*));*/
	currentPageDirectory_ = pageDirectory_;
	
	// !
	pageDirectory_->physicalAddress = phys + (reinterpret_cast<unsigned int>(pageDirectory_) - reinterpret_cast<unsigned int>(pageDirectory_->pageTablesPhysical));

	// to increase placement_address, (to place page tables before placement_address)
    for (int i = KernelHeap:: START; i < KernelHeap:: START + KernelHeap:: INITIAL_SIZE; i += Page:: SIZE)
	{
		getPage(i, true, pageDirectory_);
	}
	
	for (int i = 0; i < placement_address + Page:: SIZE; i += Page:: SIZE)
	{
		allocFrame(getPage(i, true, pageDirectory_), false, true);
	}
	
	for (int i = KernelHeap:: START; i < KernelHeap:: START + KernelHeap:: INITIAL_SIZE; i += Page:: SIZE)
	{
		allocFrame(getPage(i, true, pageDirectory_), false, true);
	}
	
	// register page fault handler
	register_isr_handler(14, pageFaultHandler);
    
    switchPaging(pageDirectory_);
    
    kernelHeap.init(KernelHeap:: START, KernelHeap:: START + KernelHeap:: INITIAL_SIZE, 0xCFFFF000, 0, 0);
   
	PageDirectory* pDir = clonePageDirectory(pageDirectory_);
	//kernelHeap.printIndex();
	
	switchPaging(pDir);
    
    enabled_ = true;    
}

void PagingManagement:: switchPaging(PageDirectory* pageDirectory)
{	
	/*
	if(pageDirectory == 0)
	{
		currentPageDirectory_ = pageDirectory_;
	}
	else
		currentPageDirectory_ = pageDirectory;*/
	
	currentPageDirectory_ = pageDirectory;
	
	__asm__ volatile("movl %0, %%eax;	\
						movl %%eax, %%cr3"
			:
			:"r"(currentPageDirectory_->physicalAddress));

	unsigned int cr3 = 0;
	__asm__ volatile("movl %%cr3, %%eax;	\
					movl %%eax, %0"
					:"=r"(cr3)
					:
					:"eax");
			
	screen.printString("CR3 - ");
	screen.printHex(cr3);
	screen.printEndl();

	unsigned int cr0 = 0;
	__asm__ volatile("movl %%cr0, %%eax;	\
					movl %%eax, %0"
					:"=r"(cr0)
					:
					:"eax");
	__asm__ volatile("movl %cr0, %eax;	\
					  orl $0x80000000, %eax;	\
					  movl %eax, %cr0");	
}

Page* PagingManagement:: getPage(unsigned int physAddress, bool makePageTable, PageDirectory *pageDirectory)
{
	if(pageDirectory == 0)
		pageDirectory = currentPageDirectory_;
		
	physAddress /= 0x1000; // 0x1000 = 2^12 , 12-offset in 32bit paging mode
	
	unsigned int tableIndex = physAddress / 1024; // Page Directory Entry Index

	if (pageDirectory->pageTables[tableIndex])
	{
		pageDirectory->pageTablesPhysical[tableIndex] |= 0x7; // TODEL ensure it is present, rw, us
		/*if(pageDirectory->pageTablesPhysical[tableIndex] & 0x00135000 == 0x00135000)
		{
			screen.printString("page - ");
			screen.printHex(pageDirectory->pageTablesPhysical[tableIndex]);
		}*/
		return &pageDirectory->pageTables[tableIndex]->pages[physAddress%1024];
	}
	else
		if(makePageTable)
		{
			unsigned int tmp;
			pageDirectory->pageTables[tableIndex] = reinterpret_cast<PageTable*>(kmalloc(sizeof(PageTable), true, &tmp));
			
			if(tmp & 0x00135000 == 0x00135000)
			{
				screen.printString("page - ");
				screen.printHex(tmp);
			}
			
			pageDirectory->pageTablesPhysical[tableIndex] = tmp | 0x7; // PRESENT, RW, US.
			memset(pageDirectory->pageTables[tableIndex], 0, (PageTable:: SIZE) * sizeof(Page));
			
			return &pageDirectory->pageTables[tableIndex]->pages[physAddress%1024];
		}
		else
		{
			// TODO:
			screen.printString("failed to get page");
			__asm__("cli");
			for(;;);
			return 0;
		}
}

int PagingManagement:: getFreeFrame()
{
	unsigned int testBit = 0;
	for (int i = 0; i < frameNumber_; i++)
	{
		if (frames_[i].getBitset() != 0xFFFFFFFF)
		{
		   // at least one bit is free
		   for (int j = 0; j < 32; j++)
		   {
			   testBit = 0x1 << j;
			   if (!(frames_[i].getBitset() & testBit))
			   {
				   return i * 32 + j;
			   }
		   }
		}
	}
	return -1;
}

bool PagingManagement:: allocFrame(Page* page, bool isKernel, bool isWriteable)
{
	if (page->frame_ == 0)
	{
		unsigned int index = getFreeFrame(); // index = i*32 +j
		if (index == static_cast<unsigned int>(-1))
		{
			screen.printString("There is no free frames!");
			return false;
		}
		
		//mark frame busy
		int i = index / 32;
		int offset = index % 32;
		frames_[i].set(offset);
		
		page->present_	= 1;
		page->rw_		= (isWriteable) ? 1 : 0; 
		page->user_		= (isKernel) ? 0 : 1;
		page->frame_	= index;
	}
	else
	{
		screen.printString("page frame != 0\n");
	}
	return true;
}

void PagingManagement:: freeFrame(Page* page)
{
	if (page->frame_)
	{
		page->frame_ = 0;
	}
}

PageTable* PagingManagement:: clonePageTable(PageTable* src, unsigned int* phys)
{
	PageTable* pTbl = reinterpret_cast<PageTable*>(kmalloc(sizeof(PageTable), true, phys));

	memset(pTbl, 0, sizeof(PageTable));
	for (unsigned int i = 0; i < PageTable:: SIZE; i++)
	{
		if (src->pages[i].frame_ == 0)
			continue;
			
		// alloc frame for page to copy
		allocFrame(&pTbl->pages[i], false, true);
		
		// copy all, except frame
		pTbl->pages[i].present_	= src->pages[i].present_;
		pTbl->pages[i].rw_		= src->pages[i].rw_;
		pTbl->pages[i].user_	= src->pages[i].user_;
		pTbl->pages[i].accessed_	= src->pages[i].accessed_;
		pTbl->pages[i].dirty_	= src->pages[i].dirty_;
		pTbl->pages[i].unused_	= src->pages[i].unused_;   
		
		copy_page_physical(src->pages[i].frame_ * Page:: SIZE, pTbl->pages[i].frame_ * Page:: SIZE);
	}

	screen.printEndl();
	return pTbl;
}

PageDirectory* PagingManagement:: clonePageDirectory(PageDirectory* src)
{
	if(src == 0)
		src = currentPageDirectory_;
	
	unsigned int phys = 0;
	PageDirectory* pDir = reinterpret_cast<PageDirectory*>(kmalloc(sizeof(PageDirectory), true, &phys));
		
	memset(pDir, 0, sizeof(PageDirectory));

	pDir->physicalAddress = phys + (reinterpret_cast<unsigned int>(pDir->pageTablesPhysical) - reinterpret_cast<unsigned int>(pDir));
		
	for(int i = 0; i < PageDirectory:: SIZE; i++)
	{
		if (!src->pageTables[i])
			continue;
		if (src->pageTables[i] == pageDirectory_->pageTables[i])
		{
			pDir->pageTables[i] = src->pageTables[i];
			pDir->pageTablesPhysical[i] = src->pageTablesPhysical[i];
		}
		else
		{
			unsigned int phys = 0;
			
			// !!! TODEL
			src->pageTablesPhysical[i] |= 0x07;
			
			pDir->pageTables[i] = clonePageTable(src->pageTables[i], &phys);
			pDir->pageTablesPhysical[i] = phys | 0x07;
		}
	}
	return pDir;
}

extern "C" void pageFaultHandler(registers regs)
{
	// The processor loads the CR2 register with the 32-bit linear address that generated the exception
	unsigned int faultingLinearAddress;
	__asm__ volatile("movl %%cr2, %0" : "=r" (faultingLinearAddress));

	//Intel docs, page 1855
	bool present	= regs.err_code & 0x1;	// 0 - non-present page; 1 - present page
	bool rw			= regs.err_code & 0x2;	// 0 - read; 1 - write
	bool us			= regs.err_code & 0x4;	// The U/S flag indicates whether the processor was executing at user mode (1) or supervisor mode (0) at the time of the exception.

	bool rsvd	= regs.err_code & 0x8;		//
	bool id		= regs.err_code & 0x10;		// 1 - the fault was caused by an instruction fetch
	
	screen.printString("Page fault: ( ");
	if (present)
		screen.printString("present, ");
	else
		screen.printString("non-present, ");
		
	if (rw) 
		screen.printString("write, ");
	else
		screen.printString("read, ");
	
	if (us)
		screen.printString("user mode, ");
	else
		screen.printString("supervisor mode, ");
	
	if (rsvd)
		screen.printString("rsvd - 1 ");
	else
		screen.printString("rsvd - 0 ");
		
	screen.printString(") at ");
	screen.printHex(faultingLinearAddress);
	screen.printEndl();
	
	__asm__("cli");
	for(;;);
}
