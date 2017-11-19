#include "primeFactors.h"

std::set<uint64_t> PrimeFactors::primeValues;

PrimeFactors::PrimeFactors(uint64_t _value)
{
	SetValue(_value);
	if (primeValues.empty())
		InitPrimes();
}

void PrimeFactors::InitPrimes()
{
	// Эти значения используются для изначального отсева мелких делителей
	// После этого мы будем 

	// Вместо решета Эратосфена будем использовать решето Аткина
	// https://habrahabr.ru/post/133037/, если есть интерес
	// https://ru.wikibooks.org/wiki/%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%BE%D0%B2/%D0%A0%D0%B5%D1%88%D0%B5%D1%82%D0%BE_%D0%90%D1%82%D0%BA%D0%B8%D0%BD%D0%B0

	uint32_t limit = UINT32_MAX / 2048 + 1;
	std::vector<bool> sieve(limit);
	for (uint16_t x = 0; x * x < limit; ++x)
		for (uint16_t y = 0; y * y < limit; ++y)
		{
			uint32_t n1 = 4 * x * x + y * y;
			uint32_t n2 = 3 * x * x + y * y;
			uint32_t n3 = 3 * x * x - y * y;
			if (n1 < limit && (n1 % 12 == 1 || n1 % 12 == 5))
				sieve[n1].flip();
			if (n2 < limit && n2 % 12 == 7)
				sieve[n2].flip();
			if (n3 < limit && x > y && n3 % 12 == 11)
				sieve[n3].flip();
		}

	for (uint32_t i = 5, i2 = i * i; i2 < limit; ++i, i2 = i * i)
		if (sieve[i])
			for (uint32_t mi2 = i2; mi2 < limit; mi2 += i2)
				sieve[mi2] = false;

	sieve[2] = true;
	sieve[3] = true;

	for (uint64_t i = 2; i < limit; ++i)
		if (sieve[i])
			primeValues.insert(i);
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
	uint64_t result = 1;
	for (auto pair : factors)
	{
		result *= (uint64_t)pow(pair.first, pair.second);
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

uint64_t PrimeFactors::GCD(uint64_t a, uint64_t b)
{
	while (b) {
		a %= b;
		uint64_t c = a;
		a = b;
		b = c;
	}
	return a;
}

uint64_t PrimeFactors::Pollard_Bent(uint64_t value, unsigned iterations_count)
{
	uint64_t
		b0 = rand() % value,
		b1 = (b0*b0 + 2) % value,
		a = b1;
	for (unsigned iteration = 0, series_len = 1; 
		iteration < iterations_count; 
		iteration++, series_len *= 2)
	{
		uint64_t g = GCD(b1 - b0, value);
		for (unsigned len = 0; len<series_len && (g == 1 && g == value); len++)
		{
			b1 = (b1*b1 + 2) % value;
			g = GCD(abs((int)(b1 - b0)), value);
		}
		b0 = a;
		a = b1;
		if (g != 1 && g != value)
			return g;
	}
	return 1;
}

void PrimeFactors::Factorize()
{
	uint64_t temp = value;
	/*
	uint64_t curFactor = 2;
	if (temp <= 3)
	{
		factors[temp] = 1;
		return;
	}
	while (curFactor * curFactor <= value && temp != 1)
	{
		if (temp % curFactor != 0)
			curFactor++;
		else while (temp % curFactor == 0)
		{
			factors[curFactor]++;
			temp /= curFactor;
		}
	}
	*/
	for (auto p : primeValues)
		while (temp % p == 0)
		{
			factors[p]++;
			temp /= p;
		}
	if (temp != 1)
	{
		factors[temp] = 1;
	}
}