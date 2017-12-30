#pragma once
#include "Exceptions.hpp"

/*
	TODO

	Part 1.
	+	Constructor
	+	Destructor
	+	Resize
	+	Reserve
	+	At
	+	operator[]
	+	Copying constructor
	+	Copying operator
	+	PushBack
	Part 2.
	+	Movement constructor
	+	Movement operator
	Part 3.
	+	Emplace
	+	EmplaceBack
	Part 4.
		Thread sync method as template parameter
	Part 5.
		Allocator as template method
*/

template<class T>
class Vector
{
public:
	Vector();
	Vector(const Vector<T>&);
	~Vector();
	Vector<T>& operator=(const Vector<T>&);
	void Resize(size_t, T& = T());
	void Reserve(size_t);
	T At(size_t) const;
	T operator[](size_t) const;
	void PushBack(T);

	Vector(Vector<T>&&);
	Vector<T>& operator=(Vector<T>&&);

	template<class ... Args>
	void Emplace(size_t, Args&&...);
	template<class ... Args>
	void EmplaceBack(Args&&...);

	size_t Size() const;

private:
	T* data;
	size_t size = 0;
	size_t capacity = 0;
	size_t spaceToAdd = 10;
};

#pragma region Part 1

template<class T>
Vector<T>::Vector()
{
	Reserve(spaceToAdd);
}

template<class T>
Vector<T>::Vector(const Vector<T>& v)
{
	size = v.size;
	capacity = v.capacity;
	data = new T[capacity];
	memcpy_s(data, sizeof(T) * size, v.data, sizeof(T) * size);
}

template<class T>
Vector<T>::~Vector()
{
	delete[] data;
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
	if (this == &v)
	{
		return *this;
	}
	delete[] data;
	size = v.size;
	capacity = v.capacity;
	data = new T[capacity];
	memcpy_s(data, sizeof(T) * size, v.data, sizeof(T) * size);
	return *this;
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
		T* temp = new T[capacity];
		memcpy_s(temp, sizeof(T) * newSize, data, sizeof(T) * newSize);
		delete[] data;
		data = temp;
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
	// TODO: Rewrite with std::move
	T* temp = new T[capacity];
	memcpy_s(temp, sizeof(T) * size, data, sizeof(T) * size);
	delete[] data;
	data = temp;
}

template<class T>
T Vector<T>::At(size_t position) const
{
	if (position >= size)
	{
		throw TryingToAccessUnexistingValue();
	}
	return data[position];
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

#pragma endregion

#pragma region Part 2

template <class T>
Vector<T>::Vector(Vector<T>&& v)
{
	capacity = v.capacity;
	size = v.size;
	data = v.data;
	v.data = nullptr;
	v.capacity = 0;
	v.size = 0;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& v)
{
	std::cout << "Movement assignment" << std::endl;
	capacity = v.capacity;
	size = v.size;
	delete[] data;
	data = v.data;
	v.data = nullptr;
	v.capacity = 0;
	v.size = 0;
	return *this;
}

#pragma endregion

#pragma region Part 3

template<class T>
template<class ... Args>
void Vector<T>::Emplace(size_t position, Args&&... arguments)
{
	Reserve(size + 1);
	memmove_s(&data[position + 1], sizeof(T) * (size - position), &data[position], sizeof(T) * (size - position));
	size++;
	data[position] = T(arguments...);
}

template<class T>
template<class ... Args>
void Vector<T>::EmplaceBack(Args&&... arguments)
{
	Reserve(size + 1);
	data[size] = T(arguments...);
	size++;
}

#pragma endregion

template<class T>
size_t Vector<T>::Size() const
{
	return size;
}