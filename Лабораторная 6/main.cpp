#include "Vector.hpp"
#include <iostream>

int main()
{
	Vector<int> v;
	for (size_t i = 0; i < 20; ++i)
	{
		v.PushBack(i);
	}
	for (size_t i = 0; i < 20; ++i)
	{
		std::cout << v.operator[](i) << std::endl;
	}
	return 0;
}