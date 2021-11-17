#ifndef HEAP_H
#define HEAP_H

#include "array.h"

// Blocks and Holes description -> Header and Footer
class Header
{
public:
	unsigned int magic_;	// Magic number, used for error checking and identification.
	bool isHole_;		// 1 - hole, 0 - block.
	unsigned int size_;	// Size of the block/hole, including header(this) and the footer.
};

class Footer
{
public:
	unsigned int magic_;
	Header *header_;			// Pointer to the block/hole header.
};

class KernelHeap
{
public:	
	KernelHeap();
	KernelHeap(unsigned int startAddress, unsigned int endAddress, unsigned int maxAddress, bool supervisor, bool readOnly);
	void init(unsigned int startAddress, unsigned int endAddress, unsigned int maxAddress, bool supervisor, bool readOnly);
	void* allocate(unsigned int size, bool pageAlign = false);
	void free(void* ptr);
	int findSmallestHole(unsigned int size, bool pageAlign);
	bool isEnabled() const;
	
	//
	void printIndex();
	
	static const unsigned int START			= 0xC0000000;
	static const unsigned int INITIAL_SIZE	= 0x02000000;
	static const unsigned int INDEX_SIZE	= 0x20000;
	static const unsigned int MAGIC			= 0x123890AB;
	static const unsigned int HEAP_MIN_SIZE	= 0x70000;
		
private:
	OrderedArray<void*> index_;
	
	unsigned int startAddress_;	// The start of our allocated space.
	unsigned int endAddress_;	// The end of our allocated space. May be expanded up to max_address.
	unsigned int maxAddress_;	// The maximum address the heap can be expanded to.
	bool supervisor_;			// Should extra pages requested by us be mapped as supervisor-only?
	bool readOnly_;				// Should extra pages requested by us be mapped as read-only?
	bool enabled_;
};
#endif // HEAP_H
