/*
 * Kernel Mode
 * Main Memory 16 Mb
 * 
 * Logical Address -> SEGMENTATION -> Linear Address -> PAGING -> Physical Address
 * 
 * Logical Address: GDT descriptor, Offset 
 * 
 * Linear Address 32 bit ...
 * 
 * |31             22|21         12|11       0|
 * | Directory Index | Table Index |  Offset  |
 * 
*/
#ifndef PAGING_MANAGEMENT_H
#define PAGING_MANAGEMENT_H

#include "page.h"
#include "screen.h"
#include "registers.h"

extern Screen screen;

class Bitset
{
public:
	void set(unsigned int offset);
	void clear(unsigned int offset);
	bool test(unsigned int offset);
	
	unsigned int getBitset();
private:
	unsigned int bitset_;
};

class PagingManagement
{
public:
	PagingManagement();
	void initialisePaging();
	
	Page* getPage(unsigned int physAddress, bool makePageTable, PageDirectory *dir);
	
	void switchPaging(PageDirectory* pageDirectory); // if 0 switch pageDirectory_
		
	int getFreeFrame(); // get first free frame
	bool allocFrame(Page* page, bool isKernel, bool isWriteable);
	void freeFrame(Page* page);
	
	PageDirectory*	clonePageDirectory(PageDirectory* src = 0);
	PageTable*		clonePageTable(PageTable* src, unsigned int* phys = 0);
	
	static unsigned int physicalToLinear(unsigned int physicalAddr); // use case: paging enabled

	bool enabled_;
//private:
	static const unsigned int PAGE_ALIGN_ADDRESS = 0xFFFFF000;
	int frameNumber_;
	Bitset* frames_;
	
	PageDirectory* pageDirectory_;			// first kernel page directory
	PageDirectory* currentPageDirectory_;	// different for each task
	
	//void copyPhysPage(unsigned int src, unsigned int dest);
};

extern "C" void pageFaultHandler(registers regs);

#endif // PAGING_MANAGEMENT_H
