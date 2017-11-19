#pragma once
#include <map>
#include <vector>
#include <set>
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

	static std::set<uint64_t> primeValues;

private:
	uint64_t value = 0;
	std::map<uint64_t, int> factors;

	uint64_t GCD(uint64_t, uint64_t);
	uint64_t Pollard_Bent(uint64_t, unsigned = 19);

	void Factorize() throw();
	void InitPrimes() throw();
};