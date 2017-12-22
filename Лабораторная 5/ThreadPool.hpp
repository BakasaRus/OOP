#pragma once
#include <queue>
#include <vector>
#include <future>
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

	void AddTask(Task);
	void Start(size_t);
	void Pause();
	void Continue();
	void Stop();
	bool IsAvailable();
	bool CanGetResult();
	R GetNextResult();
	void Wait();
	uint64_t CompletedTasksCount();

private:
	/* We assume that our thread pool will do similar tasks */
	std::queue<Task> tasks;
	std::vector<std::future<void>> workers;
	std::queue<R> results;
	TPoolState state;
	const size_t tasksPerWorker = 5;
	std::mutex resultMutex, tasksMutex;
	uint64_t tasksCompleted = 0;

	void Execute();
	bool IsWorkerReady(size_t);
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
		workers[i] = std::async(std::launch::async, &ThreadPool::Execute, this);
	}
}

template<class R>
void ThreadPool<R>::Execute()
{
	while (state != Stopped)
	{
		while (state == Executing)
		{
			Task curTask;
			{
				std::lock_guard<std::mutex> lg(tasksMutex);
				if (tasks.empty())
				{
					continue;
				}
				curTask = tasks.front();
				tasks.pop();
			}
			R res = curTask();
			{
				std::lock_guard<std::mutex> lg(resultMutex);
				results.push(res);
				++tasksCompleted;
			}
		}
	}
}

template<class R>
bool ThreadPool<R>::IsWorkerReady(size_t i)
{
	return workers[i].valid() && workers[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

template<class R>
ThreadPool<R>::~ThreadPool()
{
	Stop();
}

template<class R>
void ThreadPool<R>::AddTask(Task task)
{
	tasks.push(task);
}

template<class R>
void ThreadPool<R>::Stop()
{
	state = Stopped;
	for (size_t i = 0; i < workers.size(); ++i)
	{
		if (!workers[i].valid())
		{
			continue;
		}
		workers[i].wait();
		workers[i].get();
	}
}

template<class R>
void ThreadPool<R>::Pause()
{
	if (state != Stopped)
	{
		state = Paused;
	}
}

template<class R>
void ThreadPool<R>::Continue()
{
	if (state != Stopped)
	{
		state = Executing;
	}
}

template<class R>
bool ThreadPool<R>::IsAvailable()
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
bool ThreadPool<R>::CanGetResult()
{
	return !(results.empty());
}

template<class R>
void ThreadPool<R>::Wait()
{
	while (!tasks.empty());
}

template<class R>
uint64_t ThreadPool<R>::CompletedTasksCount()
{
	return tasksCompleted;
}