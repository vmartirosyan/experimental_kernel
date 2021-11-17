#include "kernel_heap.h"
#include "page.h"
#include "screen.h"

extern Screen screen;

KernelHeap:: KernelHeap()
{
	enabled_ = false;
	index_			= OrderedArray<void*>();
	startAddress_	= 0;
	endAddress_		= 0;
	maxAddress_		= 0;
	supervisor_		= 0;
	readOnly_		= 0;
}

KernelHeap:: KernelHeap(unsigned int startAddress, unsigned int endAddress, unsigned int maxAddress, bool supervisor, bool readOnly)
{
	init(startAddress, endAddress, maxAddress, supervisor, readOnly);
}

void KernelHeap:: init(unsigned int startAddress, unsigned int endAddress, unsigned int maxAddress, bool supervisor, bool readOnly)
{
	index_			= OrderedArray<void*>(startAddress, INDEX_SIZE);
	startAddress_	= startAddress;
	endAddress_		= endAddress;
	maxAddress_		= maxAddress;
	supervisor_		= supervisor;
	readOnly_		= readOnly;
	
	screen.printString("initialise kernel heap\nheap start address - ");
	screen.printHex(startAddress_);
	screen.printEndl();
	screen.printString("heap end address - ");
	screen.printHex(endAddress_);
	screen.printEndl();
		
	if(startAddress_ % Page:: SIZE != 0 || endAddress_ % Page:: SIZE != 0)
	{
		screen.printString("create kernel heap failed, start address or end address are not page aligned\n");
		for(;;);
	}

	startAddress_ += sizeof(void *) * INDEX_SIZE;

	if (startAddress_ & 0xFFFFF000 != 0)
	{
		startAddress_ &= 0xFFFFF000;
		startAddress_ += 0x1000;
	}
	Header *hole = reinterpret_cast<Header *>(startAddress_);
	hole->size_ = endAddress_ - startAddress_;	
	hole->magic_ = MAGIC;
	hole->isHole_ = true;

	index_.add(reinterpret_cast<void*&>(hole));
	enabled_ = true;
}

// dynamic memory allocation, size in bytes; returns 0 if no free space left;
void* KernelHeap:: allocate(unsigned int size, bool pageAlign)
{
	unsigned int sizeToAlloc = size + sizeof(Header) + sizeof(Footer); // size to allocate, including Header and Footer
	int smallestHoleIndex = findSmallestHole(sizeToAlloc, pageAlign);
	
	if (smallestHoleIndex == -1)
	{
		screen.printString("No free holes, allocation failed\n");
		return 0;
	}

	Header* holeHeader = reinterpret_cast<Header *>(index_[smallestHoleIndex]);
	unsigned int holeAddr = reinterpret_cast<unsigned int>(holeHeader);
		
	unsigned int holeSize = holeHeader->size_;	
	
	if (holeSize - sizeToAlloc < sizeof(Header) + sizeof(Footer)) // if true, the hole should not be splitted, otherwise it's not optimal
	{
		size += holeSize-sizeToAlloc; // 
		sizeToAlloc = holeSize;
	}

	// page-align, make a new hole in front of our block.
	if (pageAlign && ((holeAddr+sizeof(Header)) & 0x00000FFF))
	{
		// new location is located so that memory (between header and footer) is page aligned
		unsigned int newLocation = holeAddr + Page:: SIZE - /*(holeAddr % Page:: SIZE)*/(holeAddr & 0xFFF) - sizeof(Header);
		
		//if  new location belogs to another block, find another hole - overwise some allocated area may be overwritten
		if (newLocation < holeAddr || newLocation > holeAddr + holeHeader->size_)
		{
			// find another hole with address >= newLocation
			//smallestHoleIndex = findSmallestHole(sizeToAlloc, pageAlign, newLocation);
		}
		
		// Init hole in front  of (before) our block
		Header* holeHeader = reinterpret_cast<Header*>(holeAddr);
		holeHeader->size_ = /*newLocation - holeAddr;*/Page:: SIZE - (holeAddr % Page:: SIZE) - sizeof(Header);
		
		holeHeader->magic_ = MAGIC;
		holeHeader->isHole_  = true;
		
		Footer *holeFooter = (Footer *) (newLocation - sizeof(Footer));
		holeFooter->magic_ = MAGIC;
		holeFooter->header_   = holeHeader;
		//
		
		holeAddr = newLocation;
		holeSize = holeSize - holeHeader->size_;
		
	}
	else
	{
		// Remove the hole, it is going to be block;
		index_.erase(smallestHoleIndex);
	}
	// Init block
	Header* blockHeader	= reinterpret_cast<Header *>(holeAddr);
	blockHeader->magic_	= MAGIC;
	blockHeader->isHole_	= 0;
	blockHeader->size_		= sizeToAlloc;

	Footer* blockFooter		= reinterpret_cast<Footer *>(holeAddr + sizeof(Header) + size);
	blockFooter->magic_     = MAGIC;
	blockFooter->header_    = blockHeader;

	// If requested space is less than hole, add a new hole after allocated block
	if (holeSize - sizeToAlloc > 0)
	{
		Header* holeHeader	= reinterpret_cast<Header *>(holeAddr + sizeof(Header) + size + sizeof(Footer));
		holeHeader->magic_	= MAGIC;
		holeHeader->isHole_	= true;
		holeHeader->size_	= holeSize - sizeToAlloc;
		Footer* holeFooter = reinterpret_cast<Footer *>((unsigned int)holeHeader + holeHeader->size_ - sizeof(Footer));
		if (reinterpret_cast<unsigned int>(holeFooter) < endAddress_)
		{
			holeFooter->magic_ = MAGIC;
			holeFooter->header_ = holeHeader;
		}
		index_.add(reinterpret_cast<void*&>(holeHeader));
	}

	return (void *)((unsigned int)blockHeader+sizeof(Header));
}

void KernelHeap:: free(void* ptr)
{
	if (ptr == 0)
	{
		screen.printString("Failed to free NULL ptr");
		return;
	}

	Header* header = reinterpret_cast<Header*>((unsigned int)ptr - sizeof(Header));
	Footer* footer = reinterpret_cast<Footer*>((unsigned int)header + header->size_ - sizeof(Footer));
	if (header->magic_ != MAGIC)
	{
		screen.printString("free failed: bad heap magic\n");
		for(;;);
	}
	if (footer->magic_ != MAGIC)
	{
		screen.printString("free failed: bad heap magic\n");
		for(;;);		
	}

	header->isHole_ = true;

	bool addHole = true;
	
	// Merge left
	Footer* testFooter = reinterpret_cast<Footer*>((unsigned int)header - sizeof(Footer));
	if (testFooter->magic_ == MAGIC && testFooter->header_->isHole_ == true)
	{
		unsigned int cacheSize = header->size_;
		header = testFooter->header_;
		
		footer->header_ = header;
		header->size_ += cacheSize;
		addHole = false;
	}
	
	// Merge right
	Header *testHeader = (Header*)((unsigned int)footer + sizeof(Footer));
	if (testHeader->magic_ == MAGIC && testHeader->isHole_)
	{
		header->size_ += testHeader->size_;
		testFooter = (Footer*)((unsigned int)testHeader + testHeader->size_ - sizeof(Footer));
		footer = testFooter;
		// Find and remove this header from the index.
		unsigned int i = 0;
		while ((i < index_.size_) && (index_[i] != (void*)testHeader))
			i++;

		// Make sure we actually found the item.
		if(i >= index_.size_)
		{
			screen.printString("rigth merge, free failed\n");
			for(;;);
		}
		index_.erase(i);
	}
	
	unsigned int i = 0;
	while (i < index_.size_ && index_[i] != (void*)testHeader)
		i++;
	// check
	if (i < index_.size_)
		index_.erase(i);

	if (addHole == 1)
		index_.add(reinterpret_cast<void*&>(header));
}

/*
 * startLocation - search for hole with address >= startLocation
 */
int KernelHeap:: findSmallestHole(unsigned int size, bool pageAlign)
{
	unsigned int i = 0;
	unsigned int location;
	Header *header;

	while (i < index_.size_)
	{
		header = reinterpret_cast<Header *>(index_[i]);
		if (header->isHole_)
		{
			if (pageAlign)
			{
				location = reinterpret_cast<unsigned int>(header);
				unsigned int offset = 0;
				if (((location+sizeof(Header)) &0xFFF) != 0)
					offset = Page:: SIZE  - ((location + sizeof(Header)) % Page:: SIZE);

				int holeSize = header->size_ - offset;

				if (holeSize < size)
				{
					i++;
					continue;
				}
				else
					break;
			}
			else
				if (header->size_ >= size)
					break;
		}
		i++;
	}
	
	if (i == index_.size_)
		return -1;
		
	return i;
}

bool KernelHeap:: isEnabled() const
{
	return enabled_;
}

void KernelHeap:: printIndex()
{
	screen.printString("index: ");
	screen.printHex(index_.size_);
	screen.printEndl();
	for(int i = 0; i < index_.size_; i++)
	{
		//screen.printString("");
		screen.printHex(reinterpret_cast<unsigned int>(index_[i]));
		screen.printString(" ");
	}
}
