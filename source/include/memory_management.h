#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

unsigned int kmalloc(unsigned int size, bool align, unsigned int* phys);

void memset(void * ptr, unsigned char value, unsigned int count);

void kfree();

void* operator new(unsigned int size);

void operator delete(void*);

#endif // MEMORY_MANAGEMENT_H
