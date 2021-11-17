#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

unsigned int kmalloc(unsigned int size, bool align, unsigned int* phys = 0);

void memset(void * ptr, unsigned char value, unsigned int count);

void memcpy(void* dest, void* src, unsigned int count);

void kfree();

#endif // MEMORY_MANAGEMENT_H
