#pragma once

#include <set>
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include "exceptions.hpp"

template<class T>
class StatisticMultiset {

	// Our multiset (too straightforward)
	std::multiset<T> source;
	// Should we use cache or not
	bool useCache = false;
	// Cached minimum and maximum values
	mutable T max, min;
	// Cached average values
	mutable double avg;
	// Cached smaller and bigger values count
	mutable std::map<T, int> under, above;

	// Update maximum value
	T UpdateMax() const;
	// Update minimum value
	T UpdateMin() const;
	// Update average value
	double UpdateAvg() const;
	// Update smaller values count
	int UpdateCountUnder(T threshold) const;
	// Update bigger values count
	int UpdateCountAbove(T threshold) const;
	// Update whole cache
	void UpdateCache();

public:
	// Default constructor
	StatisticMultiset();
	// Constructor with initialization list (I'm too lazy to add data with AddNum)
	StatisticMultiset(std::initializer_list<T> initList);
	// Default destructor
	~StatisticMultiset() = default;

	// Dump our multiset
	void Dump(std::ostream& output = std::cout);
	// Dump statistics
	void DumpStats(std::ostream& output = std::cout);
	// Set caching mode (caching makes program faster if you update set rarely)
	void SetCaching(bool enabled);

	// Add single number
	void AddNum(const T&);
	// Add numbers from STL multiset
	void AddNum(const std::multiset<T>& numbers);
	// Add numbers from STL vector
	void AddNum(const std::vector<T>& numbers);
	// Add numbers from STL list
	void AddNum(const std::list<T>& numbers);
	// Add numbers from another multiset
	void AddNums(const StatisticMultiset& a_stat_set);
	// Add numbers from file
	void AddNumsFromFile(const char* filename);

	// Maximum value getter
	T GetMax() const;
	// Minimum value getter
	T GetMin() const;
	// Average value getter
	double GetAvg() const;
	// Smaller values count getter
	int GetCountUnder(T threshold) const;
	// Bigger values count getter
	int GetCountAbove(T threshold) const;
};

template<class T>
StatisticMultiset<T>::StatisticMultiset()
{
	if (!std::numeric_limits<T>::is_specialized)
	{
		throw CantUseThisType();
	}
}

template<class T>
StatisticMultiset<T>::StatisticMultiset(std::initializer_list<T> initList)
{
	StatisticMultiset();
	for (T value : initList)
	{
		source.insert(value);
	}
}

template<class T>
T StatisticMultiset<T>::UpdateMax() const
{
	return *(source.rbegin());
}

template<class T>
T StatisticMultiset<T>::UpdateMin() const
{
	return *(source.begin());
}

template<class T>
double StatisticMultiset<T>::UpdateAvg() const
{
	double sum = 0;
	for (T value : source)
	{
		sum += value;
	}
	return sum /= source.size();
}

template<class T>
int StatisticMultiset<T>::UpdateCountUnder(T threshold) const
{
	auto bound = source.lower_bound(threshold);
	int count = 0;
	for (auto it = source.begin(); it != bound; ++it)
		count++;
	return count;
}

template<class T>
int StatisticMultiset<T>::UpdateCountAbove(T threshold) const
{
	auto bound = source.upper_bound(threshold);
	int count = 0;
	for (auto it = bound; it != source.end(); ++it)
		count++;
	return count;
}

template<class T>
void StatisticMultiset<T>::UpdateCache()
{
	max = UpdateMax();
	min = UpdateMin();
	avg = UpdateAvg();
	for (T value : source)
		under[value] = UpdateCountUnder(value);
	for (T value : source)
		above[value] = UpdateCountAbove(value);
}

template<class T>
void StatisticMultiset<T>::Dump(std::ostream& output)
{
	for (T value : source)
	{
		output << value << " ";
	}
	output << std::endl;
}

template<class T>
void StatisticMultiset<T>::DumpStats(std::ostream& output)
{
	output << "Minimum: " << GetMin() << std::endl
		<< "Maximum: " << GetMax() << std::endl
		<< "Average: " << GetAvg() << std::endl;
	output << "Under:" << std::endl;
	for (T threshold : source)
		output << threshold << ": " << GetCountUnder(threshold) << std::endl;
	output << "Above:" << std::endl;
	for (T threshold : source)
		output << threshold << ": " << GetCountAbove(threshold) << std::endl;
}

template<class T>
void StatisticMultiset<T>::SetCaching(bool enabled)
{
	useCache = enabled;
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNum(const T& value)
{
	source.insert(value);
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNum(const std::multiset<T>& numbers)
{
	for (T value : numbers)
	{
		source.insert(value);
	}
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNum(const std::vector<T>& numbers)
{
	for (T value : numbers)
	{
		source.insert(value);
	}
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNum(const std::list<T>& numbers)
{
	for (T value : numbers)
	{
		source.insert(value);
	}
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNums(const StatisticMultiset& a_stat_set)
{
	for (T value : a_stat_set.source)
	{
		source.insert(value);
	}
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNumsFromFile(const char* filename)
{
	std::ifstream input(filename);
	T temp;
	while (input >> temp)
		source.insert(temp);
	if (useCache) UpdateCache();
}

template<class T>
T StatisticMultiset<T>::GetMax() const
{
	if (useCache)
		return max;
	return UpdateMax();
}

template<class T>
T StatisticMultiset<T>::GetMin() const
{
	if (useCache)
		return min;
	return UpdateMin();
}

template<class T>
double StatisticMultiset<T>::GetAvg() const
{
	if (useCache)
		return avg;
	return UpdateAvg();
}

template<class T>
int StatisticMultiset<T>::GetCountUnder(T threshold) const
{
	if (useCache)
		return under[threshold];
	return UpdateCountUnder(threshold);
}

template<class T>
int StatisticMultiset<T>::GetCountAbove(T threshold) const
{
	if (useCache)
		return above[threshold];
	return UpdateCountAbove(threshold);
}