#ifndef INCLUDE_H
#define INCLUDE_H

/*void* operator new(unsigned int n)
{
	/*
	void * const p = std::malloc(n);
	// handle p == 0
	return p;
	return 0;
}*/

void operator delete(void * p);

#endif
