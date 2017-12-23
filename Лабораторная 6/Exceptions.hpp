#pragma once
#include <exception>

class DecreasingSizeOnReserve : public std::exception
{
	const char* what() const throw()
	{
		return "You're trying to reserve space, but new size is smaller than old size";
	}
};