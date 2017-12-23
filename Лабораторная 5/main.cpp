#include "primeFactors.h"
#include <iostream>
#include "ThreadPool.hpp"

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
		std::cerr << "Whoops, can't open input file " << argv[1] << std::endl;
		return 0;
	}

	if (!fout.is_open())
	{
		std::cerr << "Whoops, can't open output file " << argv[2] << std::endl;
		return 0;
	}

	size_t tPoolSize;

	std::cout << "Set number of threads in pool: ";
	std::cin >> tPoolSize;

	ThreadPool<std::string> tPool;
	
	std::thread computing = std::thread([&]() {
		uint64_t curValue;
		PrimeFactors PF("primes.txt");
		tPool.Start(tPoolSize);
		while (fin >> curValue)
		{
			while (!tPool.IsAvailable())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			tPool.AddTask([=]() {
				return PrimeFactors(curValue).ToString();
			});

			while (tPool.CanGetResult())
			{
				fout << tPool.GetNextResult() << std::endl;
			}
		}
		tPool.Wait();
		tPool.Stop();
		while (tPool.CanGetResult())
		{
			fout << tPool.GetNextResult() << std::endl;
		}
	});

	std::string command;
	while (std::cin >> command)
	{
		if (command == "exit")
		{
			tPool.Stop();
			break;
		}
		if (command == "pause")
		{
			tPool.Pause();
			fout.close();
		}
		if (command == "resume")
		{
			tPool.Continue();
			fout.open(argv[2], std::ofstream::app);
		}
	}

	computing.join();

	return 0;
}