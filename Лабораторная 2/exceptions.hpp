#pragma once
#include <exception>

class CantUseThisType : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Can't use this type with statistic multiset";
	}
};