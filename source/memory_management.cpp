#include "memory_management.h"
#include "page.h"
#include "paging_management.h"
#include "screen.h"
#include "kernel_heap.h"

extern unsigned int kernel_end;
unsigned int placement_address;
extern KernelHeap kernelHeap;
extern PagingManagement pmmu;
extern Screen screen;

unsigned int kmalloc(unsigned int size, bool align, unsigned int* phys)
{
	if(!kernelHeap.isEnabled())
	{
		if (align && (placement_address & 0x00000FFF))
		{
			placement_address &= 0xFFFFF000;
			placement_address += Page:: SIZE;
		}
		if (phys)
		{
			*phys = placement_address;
		}
		unsigned int tmp = placement_address;
		placement_address += size;
		return tmp;
	}
	else
	{
		unsigned int placeAddr = reinterpret_cast<unsigned int>(kernelHeap.allocate(size, align));
		if(phys)
		{
			Page* page = pmmu.getPage(placeAddr, 0, 0);
			*phys = page->frame_ * Page:: SIZE + (placeAddr & 0xFFF);
		}
		return placeAddr;
	}
	
}

void memset(void * ptr, unsigned char value, unsigned int count)
{
	unsigned char * a = reinterpret_cast<unsigned char *>(ptr);
	while(count--)
	{
		*(a++) = value;
	}
}

void memcpy(void* dest, void* src, unsigned int count)
{
	unsigned char* d = (unsigned char*)dest;
	unsigned char* s = (unsigned char*)src;
	
	while(count--)
	{
		*(d++) = *(s++);
	}
}


void* operator new(unsigned int size)
{
	//if(kernelHeap != 0)
	{
		return kernelHeap.allocate(size);
	}
	return 0;
}

void operator delete(void* ptr)
{
	//if(kernelHeap != 0)
		kernelHeap.free(ptr);
}
