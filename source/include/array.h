#ifndef ARRAY_H
#define ARRAY_H

template <class T>
class Array
{
public:
	//
	Array();
	
	//
	Array(unsigned int maxSize);
	
	// place Array at the specified placeAddress, and zero memory from placeAddress - maxSize*sizeof(T)
	Array(unsigned int placeAddress, unsigned int maxSize);
	
	// place Array at the specified placeAddress, set size
	Array(unsigned int placeAddress, unsigned int maxSize, unsigned int size);
	
	virtual ~Array();
	
	void place(unsigned int placeAddress, unsigned int maxSize, unsigned int size);
	virtual int add(T& item);
	virtual void erase(int position);
	virtual void erase(int first, int last);
	T& operator[](int index);
	void operator delete(void*);
	virtual unsigned int getSize() const;

//protected:
	T* array_;
	unsigned int size_;
	unsigned int maxSize_;
};

template <class T>
class OrderedArray: public Array<T>
{
public:
	OrderedArray();
	OrderedArray(unsigned int placeAddress, unsigned int elementMaxSize);
	~OrderedArray();
		
	int add(T& item); 
	T operator[](int index);
};

#endif // ARRAY_H
