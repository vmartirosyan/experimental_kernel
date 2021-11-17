#include "linked_list.h"
#include "memory_management.h"
#include "process.h"
#include "screen.h"

// SLinkedList - singly linked list

extern Screen screen;

template <class T>
SLinkedList<T>:: SLinkedList()
{
	first_ = 0;
	last_ = first_;
}

template <class T>
SLinkedList<T>:: ~SLinkedList()
{
	// TODO
	SNode<T>* it;
	while(first_ != 0)
	{
		it = first_;
		first_ = first_->next_;
		delete it;
	}
}

template <class T>
bool SLinkedList<T>:: pushBack(T data)
{
	if(first_ == 0)
	{
		first_ = new SNode<T>(data);
		
		if(first_ == 0)
		{
			screen.printString("pushBack failed\n");
			for(;;);
		}
		//first_->next_ = 0;
		last_ = first_;
		return true;
	}
	
	last_->next_ = new SNode<T>(data);
	if(last_->next_ == 0)
	{
		screen.printString("pushBack failed\n");
		for(;;);
	}
	last_ = last_->next_;
	last_->next_ = 0;
	return true;
}

template <class T>
bool SLinkedList<T>:: pushFront(T data)
{
	return false;
}

template class SNode<void*>;
template class SNode<Process>;
template class SLinkedList<void*>;
template class SLinkedList<Process>;
