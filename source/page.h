#ifndef PAGE_H
#define PAGE_H

class Page
{
public:
   unsigned int present_	:1;   // Page present in memory
   unsigned int rw_			:1;   // Read-only if clear, readwrite if set
   unsigned int user_		:1;   // Supervisor level only if clear
   unsigned int accessed_	:1;   // Has the page been accessed since last refresh?
   unsigned int dirty_		:1;   // Has the page been written to since last refresh?
   unsigned int unused_		:7;   // Amalgamation of unused and reserved bits
   unsigned int frame_		:20;  // Frame address (shifted right 12 bits), phys page address, (the upper bits of a linear address (called the page number) determine the upper bits of the physical address (called the page frame);)
   
   Page(unsigned int present = 1, unsigned int rw = 1, unsigned int user = 1, unsigned int accessed = 1, unsigned int dirty = 0, unsigned int unused = 7, unsigned int frame = 20)
   {
	   present_ = present;
	   rw_ = rw;
	   user_ = user;
	   accessed_ = accessed;
	   dirty_ = dirty;
	   unused_ = unused;
	   frame_ = frame;
   }
   
  // Page& operator=(const Page& page);
   
   void print();   
   
   static const unsigned int SIZE = 0x1000;
};

class PageTable
{
public:
	static const int SIZE = 1024;
	Page pages[SIZE];
	
	PageTable* clone(unsigned int* phys = 0);
};

class PageDirectory
{
public:
	static const int SIZE = 1024;
	unsigned int pageTablesPhysical[SIZE];
	PageTable* pageTables[SIZE];	
	unsigned int physicalAddress;
	
	PageDirectory();
	PageDirectory* clone();
	void print();
	
private:
};
#endif //PAGE_H
