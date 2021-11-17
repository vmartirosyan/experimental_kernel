#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template <class T>
class SNode
{
public:
	SNode(): next_(0) {};
	SNode(T data): data_(data), next_(0) {};
	SNode(T data, SNode<T>* next): data_(data), next_(next) {};
	void set(T& data)
	{
		data_ = data;
	}
	
	T data_;
	SNode<T>* next_;
};

template <class T>
class SLinkedList
{
public:
	SNode<T>* first_;
	SNode<T>* last_;
	
	SLinkedList();
	~SLinkedList();
	bool pushBack(T data);
	bool pushFront(T data);

private:
};

#endif // LINKED_LIST_H
