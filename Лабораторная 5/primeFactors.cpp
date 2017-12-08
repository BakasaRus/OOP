#include "primeFactors.h"

std::vector<uint32_t> PrimeFactors::primeValues;

PrimeFactors::PrimeFactors()
{
	srand(time(NULL));
	if (primeValues.empty())
		InitPrimes();
}

PrimeFactors::PrimeFactors(std::string filename)
{
	srand(time(NULL));
	if (primeValues.empty())
		InitPrimes(filename);
}

PrimeFactors::PrimeFactors(uint64_t _value, std::string filename)
{
	srand(time(NULL));
	if (primeValues.empty())
		InitPrimes(filename);
	SetValue(_value);
}

void PrimeFactors::InitPrimes(std::string filename)
{
	/*
		Эти значения используются для изначального отсева мелких делителей

		Вместо решета Эратосфена будем использовать решето Аткина
		https://habrahabr.ru/post/133037/, если есть интерес
		https://ru.wikibooks.org/wiki/%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%BE%D0%B2/%D0%A0%D0%B5%D1%88%D0%B5%D1%82%D0%BE_%D0%90%D1%82%D0%BA%D0%B8%D0%BD%D0%B0
		
		Если ВДРУГ указан файл с простыми числами, то просто радостно их считываем
	*/
	if (filename != "")
	{
		std::ifstream primes(filename);
		uint32_t p;
		while (primes >> p)
			primeValues.push_back(p);
		return;
	}

	std::vector<bool> sieve(genLimit);
	for (uint16_t x = 0; x * x < genLimit; ++x)
		for (uint16_t y = 0; y * y < genLimit; ++y)
		{
			uint64_t n1 = 4 * x * x + y * y;
			uint64_t n2 = 3 * x * x + y * y;
			uint64_t n3 = 3 * x * x - y * y;
			if (n1 < genLimit && (n1 % 12 == 1 || n1 % 12 == 5))
				sieve[n1].flip();
			if (n2 < genLimit && n2 % 12 == 7)
				sieve[n2].flip();
			if (n3 < genLimit && x > y && n3 % 12 == 11)
				sieve[n3].flip();
		}

	for (uint32_t i = 5, i2 = i * i; i2 < genLimit; ++i, i2 = i * i)
		if (sieve[i])
			for (uint32_t mi2 = i2; mi2 < genLimit; mi2 += i2)
				sieve[mi2] = false;

	sieve[2] = true;
	sieve[3] = true;

	for (uint64_t i = 2; i < genLimit; ++i)
		if (sieve[i])
			primeValues.push_back(i);
}

void PrimeFactors::SetValue(uint64_t _value)
{
	value = _value;
	factors.clear();
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

#pragma region Pollard Rho from SO (https://stackoverflow.com/questions/37083215/c-brent-pollard-rho-algorithm-infinite-loop)
#define min(a,b) (a<b?a:b)
#define abs(x,y) (x > y? x - y : y - x)

uint64_t PrimeFactors::GCD(uint64_t m, uint64_t n) {
	while (true) {
		uint64_t r = m % n;
		if (r == 0) {
			return n;
		}
		m = n;
		n = r;
	}
}
uint64_t PrimeFactors::F(uint64_t y, uint64_t c, uint64_t n) {
	y = (y * y) % n;
	y += c;
	if (y < c)
		y += (std::numeric_limits<uint32_t>::max() - n) + 1;
	y %= n;
	return y;
}

uint64_t PrimeFactors::Pollard_Rho(uint64_t n)
{
	if (n == 1) return n;
	if (n % 2 == 0) return 2;

	uint64_t y = rand() % n;
	uint64_t x;
	uint64_t ys = y;
	uint64_t c;
	do c = rand() % n; while (c == 0 || c == n - 2);
	uint64_t m = 1000;
	uint64_t d = 1;
	uint64_t q = 1;
	uint64_t r = 1;
	do
	{
		x = y;
		for (uint64_t i = 0; i <= r; i++)
			y = F(y, c, n);

		uint64_t j = 0;
		do
		{
			ys = y;
			for (uint64_t i = 0; i <= min(m, r - j); i++)
			{
				y = F(y, c, n);
				q *= (abs(x, y) % n);
			}
			d = GCD(q, n);
			j += m;
		} while (j < r && d == 1);
		r *= 2;
	} while (d == 1);
	if (d == n)
	{
		do
		{
			ys = F(ys, c, n);
			d = GCD(abs(x, ys), n);
		} while (d == 1);
	}
	return d;
}
#pragma endregion

#pragma region Fermat Primality Test (https://habrahabr.ru/post/205318/)
uint64_t PrimeFactors::Mul(uint64_t a, uint64_t b, uint64_t m) {
	if (!(a * b))
		return 0;
	if (b == 1)
		return a;
	if (b % 2 == 0) {
		uint64_t t = Mul(a, b / 2, m);
		return (2 * t) % m;
	}
	return (Mul(a, b - 1, m) + a) % m;
}

uint64_t PrimeFactors::Pows(uint64_t a, uint64_t b, uint64_t m) {
	if (b == 0)
		return 1;
	if (b % 2 == 0) {
		uint64_t t = Pows(a, b / 2, m);
		return Mul(t, t, m) % m;
	}
	return (Mul(Pows(a, b - 1, m), a, m)) % m;
}

bool PrimeFactors::IsPrime(uint64_t number)
{
	if (number == 2)
		return true;
	for (int i = 0; i < 100; i++) {
		uint64_t a = ((rand() * rand()) % (number - 2)) + 2;
		if (GCD(a, number) != 1)
			return false;
		if (Pows(a, number - 1, number) != 1)
			return false;
	}
	return true;
}

#pragma endregion

void PrimeFactors::Factorize()
{
	uint64_t temp = value;
	if (temp < 2)
	{
		factors[temp]++;
		return;
	}
	for (auto p : primeValues)
	{
		while (temp % p == 0)
		{
			factors[p]++;
			temp /= p;
		}
		if (temp == 1)
		{
			return;
		}
	}
	while (temp > 1 && !IsPrime(temp))
	{
		uint64_t a = 1;
		for (int i = 0; i < 5 && (a == 1 || a == temp); ++i)
			a = Pollard_Rho(temp);
		factors[a]++;
		temp /= a;
	}
	factors[temp]++;
}