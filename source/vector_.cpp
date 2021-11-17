#include "kernel_heap.h"
#include "memory_management.h"
#include "vector.h"
#include "screen.h"
#include "vfs/fs.h"
#include "vfs/initrd_fs.h"

extern Screen screen;
extern KernelHeap kernelHeap;
extern unsigned int placement_address;

template <class T>
vector<T>:: vector()
{
	elementMaxSize_ = 0;
	array_ = 0;
	size_ = 0;
}

template <class T>
vector<T>:: vector(unsigned int placeAddress, unsigned int elementMaxSize, unsigned int size)
{
	elementMaxSize_ = elementMaxSize;
	array_ = (T*)placeAddress;
	memset(array_, 0, elementMaxSize_ * sizeof(T));
	size_ = size;
}

/*
 * create vector with "size" zeroed elements
 */
template <class T>
vector<T>:: vector(unsigned int elementMaxSize, unsigned int size)
{
	elementMaxSize_ = elementMaxSize;
	array_ = reinterpret_cast<T*>(kernelHeap.allocate(elementMaxSize_ * sizeof(T)));
	memset(array_, 0, elementMaxSize_ * sizeof(T));
	size_ = size;
}

template <class T>
vector<T>:: ~vector()
{
	// TODO:
	// KFREE ?
}

template <class T>
void vector<T>:: allocate(unsigned int placeAddress, unsigned int elementMaxSize, unsigned int size)
{
	elementMaxSize_ = elementMaxSize;
	array_ = (T*)placeAddress;
	size_ = size;
}

template <class T>
int vector<T>:: push_back(T item)
{
	array_[size_ - 1] = item;
	size_++;
}

template <class T>
void vector<T>:: erase(int position)
{
	for (int i = position; i < size_ - 1; i++)
	{
		array_[i] = array_[i+1];
	}
	size_--;
}

template <class T>
void vector<T>:: erase(int first, int last) // TODO: test it :/
{
	int i = 0;
	for (i = first; i+last+1 < size_; i++)
	{
		array_[i] = array_[i+last+1];
	}
	size_ = i;
}

template <class T>
T& vector<T>:: operator[](int index)
{
	if(index < size_)
		return array_[index];
	return array_[0];
}

template <class T>
unsigned int vector<T>:: size()
{
	return size_;
}

template class vector<FileNode>;
template class vector<InitrdFs:: FileHeader>;
