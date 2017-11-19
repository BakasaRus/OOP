#include "primeFactors.h"
#include <iostream>
#include <fstream>
#include <cassert>

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
	while (fin >> curValue)
	{
		std::clog << curValue << std::endl;
		PrimeFactors PF(curValue);
		fout << curValue << " = " << PF.ToString() << std::endl;
	}

	fout << std::endl;

	for (auto p : PrimeFactors::primeValues)
		fout << p << std::endl;

	return 0;
}