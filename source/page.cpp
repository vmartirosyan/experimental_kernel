#include "page.h"
#include "memory_management.h"
#include "screen.h"

extern Screen screen;

//
/*
Page& Page:: operator=(const Page& page)
{
   present_	= page.present_;
   rw_		= page.rw_;
   user_	= page.user_;
   accessed_	= page.accessed_;
   dirty_	= page.dirty_;
   unused_	= page.unused_;
   //frame_ = page.frame_;
}*/

void Page:: print()
{
	screen.printString("Page(");
	screen.printInt(present_);
	screen.printString(", ");
	screen.printInt(rw_);
	screen.printString(", ");
	screen.printInt(user_);
	screen.printString(", ");
	screen.printInt(accessed_);
	screen.printString(", ");
	screen.printInt(dirty_);
	screen.printString(", ");
	screen.printInt(unused_);
	screen.printString(", ");
	screen.printInt(frame_);
	screen.printString(")");
}

//PageTable


// PageDirectory

void PageDirectory:: print()
{
	for(int i = 0; i < PageDirectory:: SIZE; i++)
	{		
		for(int j = 0; j < PageTable:: SIZE; j++) 
		{			
			(pageTables[i]->pages[j]).print();
		}
		screen.printEndl();
	}
}
