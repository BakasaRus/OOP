#pragma once
#include <exception>

class CantUseThisType : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Can't use this type with statistic multiset";
	}
};

class FileNotFound : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "File not found";
	}
};