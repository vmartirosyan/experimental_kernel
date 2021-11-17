#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

class Descriptor
{
public:
	Descriptor(unsigned long limit = 0, unsigned long base = 0, unsigned char access = 0, unsigned char granularity = 0)
	{
		limitLow_	= (limit & 0xFFFF);
		baseLow_	= (base & 0xFFFF);
		baseMiddle_	= (base >> 16) & 0xFF;
		access_		= access;
		
		granularity_	= (limit >> 16) & 0x0F;
		granularity_	|= (granularity & 0xF0);
		
		baseHigh_	= (base >> 24) & 0xFF;
	}
protected:
   unsigned short limitLow_;           // 15 0
   unsigned short baseLow_;            // 31 16
   unsigned char  baseMiddle_;         // 39 32
/*
* P segment present 0 / 1
* DPL privilege level, 2 bits
* DT descriptor type 
*/
   unsigned char  access_;	//

   unsigned char  granularity_;
   unsigned char  baseHigh_;           	
} __attribute__((packed));

#endif //DESCRIPTOR_H
