#include "primeFactors.h"
#include <iostream>
#include "ThreadPool.hpp"

std::string Compute(uint64_t value)
{
	return PrimeFactors(value).ToString();
}

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

	ThreadPool<std::string> tPool(10);

	while (fin >> curValue)
	{
		while (!tPool.IsAvailable())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		tPool.AddTask(std::bind(Compute, curValue));
		while (tPool.CanGetResult())
		{
			fout << tPool.GetNextResult() << std::endl;
			fout.flush();
		}
	}
	tPool.Pause();
	fout << "PAUSED" << std::endl;
	fout.flush();
	while (tPool.CanGetResult())
	{
		fout << tPool.GetNextResult() << std::endl;
		fout.flush();
	}
	tPool.Continue();
	fout << "CONTINUED" << std::endl;
	fout.flush();
	tPool.Wait();
	while (tPool.CanGetResult())
	{
		fout << tPool.GetNextResult() << std::endl;
		fout.flush();
	}
	return 0;
}