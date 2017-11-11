#pragma once
#include <map>
#include <algorithm>
#include <string>

class PrimeFactors
{

public:
	PrimeFactors() = default;
	PrimeFactors(uint64_t);
	~PrimeFactors() = default;

	void SetValue(uint64_t) throw();
	uint64_t GetValue() const throw();
	bool IsCorrect() const throw();
	std::string ToString() const throw();

private:
	uint64_t value = 0;
	std::map<uint64_t, int> factors;
	void Factorize() throw();
};