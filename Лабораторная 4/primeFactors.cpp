#include "primeFactors.h"

PrimeFactors::PrimeFactors(uint64_t _value)
{
	SetValue(_value);
}

void PrimeFactors::SetValue(uint64_t _value)
{
	value = _value;
	Factorize();
}

uint64_t PrimeFactors::GetValue() const
{
	return value;
}

bool PrimeFactors::IsCorrect() const
{
	uint64_t result = 0;
	for (auto pair : factors)
	{
		result += pow(pair.first, pair.second);
	}
	return value == result;
}

std::string PrimeFactors::ToString() const
{
	std::string result;
	bool isFirst = true;

	for (auto pair : factors)
	{
		if (isFirst)
			isFirst = false;
		else
			result += " * ";

		if (pair.second == 1)
			result += std::to_string(pair.first);
		else
			result += std::to_string(pair.first) + "^" + std::to_string(pair.second);
	}

	return result;
}

void PrimeFactors::Factorize()
{
	uint64_t temp = value;
	uint64_t curFactor = 2;
	while (temp > 1)
	{
		while (temp % curFactor != 0)
			curFactor++;
		while (temp % curFactor == 0)
		{
			factors[curFactor]++;
			temp /= curFactor;
		}
	}
}