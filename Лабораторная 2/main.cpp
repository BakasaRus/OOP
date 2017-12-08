#include "statisticMultiset.hpp"
#include <chrono>

int main()
{
	std::ofstream result("result.txt");

	StatisticMultiset<int> SMS{46, 71, 90, -32};
	SMS.SetCaching(false);

	std::list<int> List{89, 55, 0, 43, 22, -90, 1};
	SMS.AddNum(List.begin(), List.end());
	SMS.Dump(result);
	SMS.DumpStats(result);

	StatisticMultiset<int> SMS2{ 1000, 2000, 3000 };
	SMS.AddNums(SMS2);
	SMS.Dump(result);
	SMS.DumpStats(result);
	
	SMS.AddNumsFromFile("data.txt");
	SMS.Dump(result);

	// Stress test with ~250 numbers to compare non-caching and caching mode
	std::chrono::high_resolution_clock::time_point start, finish;
	std::chrono::duration<double, std::milli> duration;
	SMS.Dump(result);
	start = std::chrono::high_resolution_clock::now();
	SMS.DumpStats(result);
	finish = std::chrono::high_resolution_clock::now();
	duration = finish - start;
	result << "Duration: " << duration.count() << " ms" << std::endl;
	
	StatisticMultiset<double> SMSDouble{99.6, 14.8};
	SMSDouble.DumpStats(result);

	try
	{
		StatisticMultiset<std::string> test;
	}
	catch (CantUseThisType& e)
	{
		std::cerr << e.what();
	}

	return 0;
}