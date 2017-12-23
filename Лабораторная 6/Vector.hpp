#pragma once
#include "Exceptions.hpp"

template<class T>
class Vector
{
public:
	Vector();
	Vector(const Vector<T>&);
	~Vector();
	void Resize(size_t, T& = T());
	void Reserve(size_t);
	T At(size_t);
	T operator[](size_t) const;
	void PushBack(T);
	void operator=(const Vector<T>&);

private:
	T* data;
	size_t size = 0;
	size_t capacity = 0;
	size_t spaceToAdd = 10;
};

template<class T>
Vector<T>::Vector()
{
	Reserve(spaceToAdd);
}

template<class T>
Vector<T>::~Vector()
{
	delete data;
}

template<class T>
T Vector<T>::operator[](size_t position) const
{
	return data[position];
}

template<class T>
void Vector<T>::PushBack(T value)
{
	if (size == capacity)
	{
		Reserve(size + 1);
	}
	data[size] = value;
	size++;
}

template<class T>
void Vector<T>::Resize(size_t newSize, T& value)
{
	if (size == newSize)
	{
		return;
	}
	else if (newSize > size)
	{
		Reserve(newSize);
		for (size_t i = size; i < newSize; ++i)
		{
			data[i] = value;
		}
		size = newSize;
	}
	else
	{
		// I don't know if this will clean memory whether T is an object (but it should)
		capacity = newSize / spaceToAdd * spaceToAdd + spaceToAdd;
		data = (R*)realloc(data, capacity);
		size = newSize;
	}
}

template<class T>
void Vector<T>::Reserve(size_t newSize)
{
	if (newSize < size)
	{
		throw DecreasingSizeOnReserve();
	}
	if (capacity >= newSize && capacity - newSize < spaceToAdd)
	{
		// Nothing to do because we already have enough space
		return;
	}
	capacity = newSize / spaceToAdd * spaceToAdd;
	if (capacity < newSize)
	{
		capacity += spaceToAdd;
	}
	data = (T*)realloc(data, capacity);
}