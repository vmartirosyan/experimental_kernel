#ifndef VECTOR_H
#define VECTOR_H

template <class T>
class vector
{
public:
	vector();
	vector(unsigned int placeAddress, unsigned int elementMaxSize, unsigned int size);
	vector(unsigned int elementMaxSize, unsigned int size = 0);
	~vector();
	
	// allocate vector on the specified address
	void allocate(unsigned int placeAddress, unsigned int elementMaxSize, unsigned int size=0);
	
	int push_back(T item);
	void erase(int position);
	void erase(int first, int last);
	T& operator[](int index);
	unsigned int size();
//private:
	T* array_;

	unsigned int elementMaxSize_;
private:
		unsigned int size_;
};

#endif // VECTOR_H
