#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

enum TPoolState { Executing, Paused, Stopped };

template<class R>
class ThreadPool
{
public:
	ThreadPool() = default;
	ThreadPool(size_t);
	~ThreadPool();

	typedef std::function<R()> Task;

	void AddTask(Task) throw();
	void Start(size_t) throw();
	void Pause() throw();
	void Continue() throw();
	void Stop() throw();
	bool IsAvailable() const throw();
	bool CanGetResult() const throw();
	R GetNextResult() throw();
	void Wait() const throw();
	uint64_t CompletedTasksCount() const throw();

private:
	std::queue<Task> tasks;
	std::vector<std::thread> workers;
	std::queue<R> results;
	TPoolState state;
	const size_t tasksPerWorker = 5;
	std::mutex resultMutex, tasksMutex;
	uint64_t tasksCompleted = 0;
};

template<class R>
ThreadPool<R>::ThreadPool(size_t workersCount)
{
	Start(workersCount);
}

template<class R>
void ThreadPool<R>::Start(size_t workersCount)
{
	workers.resize(workersCount);
	state = Executing;
	for (size_t i = 0; i < workersCount; ++i)
	{
		workers[i] = std::thread([&]() 
		{
			while (state != Stopped)
			{
				while (state == Executing)
				{
					Task curTask;
					{
						std::lock_guard<std::mutex> lg(tasksMutex);
						if (!tasks.empty())
						{
							curTask = tasks.front();
							tasks.pop();
						}
						else
						{
							continue;
						}
					}
					R res = curTask();
					{
						std::lock_guard<std::mutex> lg2(resultMutex);
						results.push(res);
						++tasksCompleted;
					}
				}
			}
		});
	}
}

template<class R>
ThreadPool<R>::~ThreadPool()
{
	if (state != Stopped)
	{
		Stop();
	}
}

template<class R>
void ThreadPool<R>::AddTask(Task task) throw()
{
	std::lock_guard<std::mutex> lg(tasksMutex);
	tasks.push(task);
}

template<class R>
void ThreadPool<R>::Stop() throw()
{
	state = Stopped;
	for (size_t i = 0; i < workers.size(); ++i)
	{
		if (!workers[i].joinable())
		{
			continue;
		}
		workers[i].join();
	}
}

template<class R>
void ThreadPool<R>::Pause() throw()
{
	if (state != Stopped)
	{
		state = Paused;
	}
}

template<class R>
void ThreadPool<R>::Continue() throw()
{
	if (state != Stopped)
	{
		state = Executing;
	}
}

template<class R>
bool ThreadPool<R>::IsAvailable() const throw()
{
	return (tasks.size() < workers.size() * tasksPerWorker);
}

template<class R>
R ThreadPool<R>::GetNextResult()
{
	std::lock_guard<std::mutex> lg(resultMutex);
	R res = results.front();
	results.pop();
	return res;
}

template<class R>
bool ThreadPool<R>::CanGetResult() const throw()
{
	return !(results.empty());
}

template<class R>
void ThreadPool<R>::Wait() const throw()
{
	while (!tasks.empty());
}

template<class R>
uint64_t ThreadPool<R>::CompletedTasksCount() const throw()
{
	return tasksCompleted;
}