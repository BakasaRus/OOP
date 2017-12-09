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
	T UpdateMax() const throw();

	// Update minimum value
	T UpdateMin() const throw();

	// Update average value
	double UpdateAvg() const throw();

	// Update smaller values count
	int UpdateCountUnder(T threshold) const throw();

	// Update bigger values count
	int UpdateCountAbove(T threshold) const throw();

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
	void Dump(std::ostream& output = std::cout) throw();

	// Dump statistics
	void DumpStats(std::ostream& output = std::cout) throw();

	// Set caching mode (caching makes program faster if you update set rarely)
	void SetCaching(bool enabled) throw();

	// Add single number
	void AddNum(const T&) throw();

	// Add numbers from STL containers
	// Many thanks to http://edu.tochlab.net/forum/viewtopic.php?f=4&t=25#p158
	template <class It>
	void AddNum(It begin, It end) throw();

	// Add numbers from another multiset
	void AddNums(const StatisticMultiset& a_stat_set) throw();

	// Add numbers from file
	void AddNumsFromFile(const char* filename) throw();

	// Maximum value getter
	T GetMax() const throw();

	// Minimum value getter
	T GetMin() const throw();

	// Average value getter
	double GetAvg() const throw();

	// Smaller values count getter
	int GetCountUnder(T threshold) const throw();

	// Bigger values count getter
	int GetCountAbove(T threshold) const throw();
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
T StatisticMultiset<T>::UpdateMax() const throw()
{
	return *(source.rbegin());
}

template<class T>
T StatisticMultiset<T>::UpdateMin() const throw()
{
	return *(source.begin());
}

template<class T>
double StatisticMultiset<T>::UpdateAvg() const throw()
{
	double sum = 0;
	for (T value : source)
	{
		sum += value;
	}
	return sum /= source.size();
}

template<class T>
int StatisticMultiset<T>::UpdateCountUnder(T threshold) const throw()
{
	auto bound = source.lower_bound(threshold);
	int count = 0;
	for (auto it = source.begin(); it != bound; ++it)
		count++;
	return count;
}

template<class T>
int StatisticMultiset<T>::UpdateCountAbove(T threshold) const throw()
{
	auto bound = source.upper_bound(threshold);
	int count = 0;
	for (auto it = bound; it != source.end(); ++it)
		count++;
	return count;
}

template<class T>
void StatisticMultiset<T>::UpdateCache() throw()
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
void StatisticMultiset<T>::Dump(std::ostream& output) throw()
{
	for (T value : source)
	{
		output << value << " ";
	}
	output << std::endl;
}

template<class T>
void StatisticMultiset<T>::DumpStats(std::ostream& output) throw()
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
void StatisticMultiset<T>::SetCaching(bool enabled) throw()
{
	useCache = enabled;
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNum(const T& value) throw()
{
	source.insert(value);
	if (useCache) UpdateCache();
}

template<class T>
template<class It> 
void StatisticMultiset<T>::AddNum(It begin, It end) throw()
{
	for (It cur = begin; cur != end; ++cur)
	{
		source.insert(*cur);
	}
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNums(const StatisticMultiset& a_stat_set) throw()
{
	if (source.begin() == a_stat_set.source.begin())
	{
		return;
	}
	for (T value : a_stat_set.source)
	{
		source.insert(value);
	}
	if (useCache) UpdateCache();
}

template<class T>
void StatisticMultiset<T>::AddNumsFromFile(const char* filename) throw(FileNotFound)
{
	std::ifstream input(filename);
	if (!input.is_open())
	{
		throw FileNotFound();
		return;
	}
	T temp;
	while (input >> temp)
		source.insert(temp);
	if (useCache) UpdateCache();
}

template<class T>
T StatisticMultiset<T>::GetMax() const throw()
{
	if (useCache)
		return max;
	return UpdateMax();
}

template<class T>
T StatisticMultiset<T>::GetMin() const throw()
{
	if (useCache)
		return min;
	return UpdateMin();
}

template<class T>
double StatisticMultiset<T>::GetAvg() const throw()
{
	if (useCache)
		return avg;
	return UpdateAvg();
}

template<class T>
int StatisticMultiset<T>::GetCountUnder(T threshold) const throw()
{
	if (useCache)
		return under[threshold];
	return UpdateCountUnder(threshold);
}

template<class T>
int StatisticMultiset<T>::GetCountAbove(T threshold) const throw()
{
	if (useCache)
		return above[threshold];
	return UpdateCountAbove(threshold);
}