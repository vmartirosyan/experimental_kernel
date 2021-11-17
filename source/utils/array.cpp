#include "array.h"
#include "kernel_heap.h"
#include "memory_management.h"
#include "screen.h"

#include "vfs/fs.h"
#include "vfs/initrd_fs.h"

extern Screen screen;
extern unsigned int placement_address;
extern KernelHeap kernelHeap;
// Array
template <class T>
Array<T>:: Array()
{
	maxSize_ = 0;
	array_ = 0;
	size_ = 0;
}

template <class T>
Array<T>:: Array(unsigned int maxSize)
{
	maxSize_ = maxSize;
	array_ = reinterpret_cast<T*>(kernelHeap.allocate(maxSize_ * sizeof(T)));
	memset(array_, 0, maxSize_ * sizeof(T));
	size_ = 0;
}

template <class T>
Array<T>:: Array(unsigned int placeAddress, unsigned int maxSize)
{
	maxSize_ = maxSize;
	array_ = reinterpret_cast<T*>(placeAddress);
	memset(array_, 0, maxSize_ * sizeof(T));
	size_ = 0;
}

template <class T>
Array<T>:: Array(unsigned int placeAddress, unsigned int maxSize, unsigned int size)
{
	place(placeAddress, maxSize, size);
}

template <class T>
Array<T>:: ~Array()
{
	// TODO:
	// KFREE
}

template <class T>
void Array<T>:: place(unsigned int placeAddress, unsigned int maxSize, unsigned int size)
{
	maxSize_ = maxSize;
	array_ = reinterpret_cast<T*>(placeAddress);
	size_ = size;
}

template <class T>
int Array<T>:: add(T& item)
{
	if(size_ != maxSize_)
	{
		array_[size_] = item;
		size_++;
		return size_;
	}
	return -1;
}

template <class T>
void Array<T>:: erase(int position)
{
	for (int i = position; i < size_ - 1; i++)
	{
		array_[i] = array_[i+1];
	}
	size_--;
}

template <class T>
void Array<T>:: erase(int first, int last) // TODO: test it :/
{
	int i = 0;
	for (i = first; i+last+1 < size_; i++)
	{
		array_[i] = array_[i+last+1];
	}
	size_ = i;
}

template <class T>
T& Array<T>:: operator[](int index)
{
	if(index < size_)
		return array_[index];
	//return 0; throw exeption
}

template <class T>
void Array<T>:: operator delete(void*)
{
	
}

template <class T>
unsigned int Array<T>:: getSize() const
{
	return size_;
}

// Ordered Array

template <class T>
OrderedArray<T>:: OrderedArray(): Array<T>()
{
	
}

template <class T>
OrderedArray<T>:: OrderedArray(unsigned int placeAddress, unsigned int maxSize): Array<T>(placeAddress, maxSize)
{

}

template <class T>
OrderedArray<T>:: ~OrderedArray()
{
	
}

template <class T>
int OrderedArray<T>:: add(T& item)
{
	unsigned int i = 0;
   
	while (i < Array<T>:: size_ && (Array<T>:: array_[i] < item))
		i++;
   
	if (i == Array<T>:: size_) // add at the end of the array.
	{

		Array<T>:: array_[i] = item;
		Array<T>:: size_++;
	}
	else
	{
		T tmp = Array<T>:: array_[i];
		Array<T>:: array_[i] = item;
		T tmp2 = 0;
		while (i < Array<T>:: size_)
		{
			i++;
			tmp2 = Array<T>:: array_[i];
			Array<T>:: array_[i] = tmp;
			tmp = tmp2;
		}
		Array<T>:: size_++;
	}
}

template <class T>
T OrderedArray<T>:: operator[](int index)
{
	if(index < Array<T>:: size_)
		return Array<T>:: array_[index];
	return 0;
}

template class Array<void*>;
template class OrderedArray<void*>;
template class Array<InitrdFs:: FileHeader>;
template class Array<FileNode>;
