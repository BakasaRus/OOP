#pragma once
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>

class PrimeFactors
{

public:
	PrimeFactors();
	PrimeFactors(std::string);
	PrimeFactors(uint64_t, std::string = "");
	~PrimeFactors() = default;

	void SetValue(uint64_t) throw();
	uint64_t GetValue() const throw();
	bool IsCorrect() const throw();
	std::string ToString() const throw();

	static std::vector<uint32_t> primeValues;

private:
	uint64_t value = 0;
	uint32_t genLimit = 1 << 20;
	std::map<uint64_t, int> factors;

	uint64_t Pollard_Rho(uint64_t) throw();
	uint64_t GCD(uint64_t, uint64_t) throw();
	uint64_t F(uint64_t, uint64_t, uint64_t);
	
	bool IsPrime(uint64_t) throw();
	uint64_t Mul(uint64_t, uint64_t, uint64_t) throw();
	uint64_t Pows(uint64_t, uint64_t, uint64_t) throw();

	void Factorize() throw();
	void InitPrimes(std::string = "") throw();
};