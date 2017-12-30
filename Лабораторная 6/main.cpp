#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)

#include "Vector.hpp"
#include <iostream>

int main()
{
	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);

	Vector<int> v;
	for (size_t i = 0; i < 20; ++i)
	{
		v.PushBack(i);
	}
	for (size_t i = 0; i < v.Size(); ++i)
	{
		std::cout << v[i] << std::endl;
	}

	Vector<int> v2;
	v2 = std::move(v);
	for (size_t i = 0; i < v2.Size(); ++i)
	{
		std::cout << v2[i] << std::endl;
	}
	for (size_t i = 0; i < v.Size(); ++i)
	{
		std::cout << v[i] << std::endl;
	}
	
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

	_CrtMemDumpAllObjectsSince(&_ms);

	system("pause");
	return 0;
}