#include "paging_management.h"
#include "memory_management.h"
#include "page.h"
#include "task.h"

extern PagingManagement pmmu;
extern unsigned int initialEsp;
void Sheduler:: changeStack(void* newStartAddr, unsigned int newSize)
{
	// stack grows up
	for (unsigned int i = reinterpret_cast<unsigned int>(newStartAddr); i >= reinterpret_cast<unsigned int>(newStartAddr) - newSize; i -= Page:: SIZE)
	{
		pmmu.allocFrame(pmmu.getPage(i, true, pmmu.currentPageDirectory_), false, true);	// user mode, writable
	}
	
	// flush TLB
	unsiged int pdAddr;
	__asm__("mov %0, %%cr3": "=r"(pdAddr));
	__asm__("mov %%cr3, %0": "r"(pdAddr));
	
	unsigned int oldEsp;
	__asm__("movl %0, %esp": "=r"(oldEsp));
	unsigned int oldEbp;
	__asm__("movl %0, %esb": "=r"(oldEbp));
	
	unsigned int offset = reinterpret_cast<unsigned int>(newStartAddr) - initialEsp;
	
	unsigned int newEsp = newStartAddr - (initialEsp - oldEsp);//oldEsp + offset;
	unsigned int newEbp = newStartAddr + (oldEbp - initEsp);//oldEbp + offset;
	
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
	__asm__("mov %%esp, %0" : : "r" (newEsp));
	__asm__("mov %%ebp, %0" : : "r" (newEbp));
}

  // Backtrace through the original stack, copying new values into
  // the new stack.
  for(i = (u32int)new_stack_start; i > (u32int)new_stack_start-size; i -= 4)
  {
    u32int tmp = * (u32int*)i;
    // If the value of tmp is inside the range of the old stack, assume it is a base pointer
    // and remap it. This will unfortunately remap ANY value in this range, whether they are
    // base pointers or not.
    if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    {
      tmp = tmp + offset;
      u32int *tmp2 = (u32int*)i;
      *tmp2 = tmp;
    }
  }

