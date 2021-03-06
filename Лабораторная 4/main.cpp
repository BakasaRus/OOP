#include "primeFactors.h"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Input and/or output filenames are not defined (first is input, second is output)" << std::endl;
		return 0;
	}

	std::ifstream fin(argv[1]);
	std::ofstream fout(argv[2]);

	if (!fin.is_open())
	{
		std::cerr << "Whoops, can't open file " << argv[1] << std::endl;
		return 0;
	}

	if (!fout.is_open())
	{
		std::cerr << "Whoops, can't open file " << argv[2] << std::endl;
		return 0;
	}
	uint64_t curValue;
	PrimeFactors PF("primes.txt");

	while (fin >> curValue)
	{
		std::clog << curValue << std::endl;
		PF.SetValue(curValue);
		fout << std::setw(20) << curValue << " = " << PF.ToString() << std::endl;
	}
	return 0;
}