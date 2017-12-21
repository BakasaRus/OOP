#pragma once
#include <queue>
#include <vector>
#include <future>
#include <chrono>

enum TPoolState { Executing, Paused, Stopped };

template<class R, class... Args>
class ThreadPool
{
public:
	ThreadPool(size_t);
	~ThreadPool();

	typedef std::function<R()> Task;

	void AddTask(Task);
	void Pause();
	void Continue();
	void Stop();
	bool IsAvailable();
	bool CanGetResult();
	R GetNextResult();
	;void Wait();

private:
	/* We assume that our thread pool will do similar tasks */
	std::queue<Task> tasks;
	std::vector<std::future<R>> workers;
	std::future<void> controller;
	std::queue<R> results;
	TPoolState state;
	const size_t tasksPerWorker = 5;
	std::mutex resultMutex;
	void Execute();
	bool IsWorkerReady(size_t);
};

template<class R, class... Args>
ThreadPool<R, Args...>::ThreadPool(size_t workersCount)
{
	workers.resize(workersCount);
	state = Executing;
	controller = std::async(std::launch::async, &ThreadPool::Execute, this);
}

template<class R, class... Args>
void ThreadPool<R, Args...>::Execute()
{
	while (state != Stopped)
	{
		for (uint32_t i = 0; i < workers.size(); ++i)
		{
			if (!workers[i].valid() && !tasks.empty())
			{
				workers[i] = std::async(std::launch::async, tasks.front());
				tasks.pop();
			}
			else if (IsWorkerReady(i))
			{
				std::lock_guard<std::mutex> lg(resultMutex);
				results.push(workers[i].get());
			}
		}
	}
}

template<class R, class... Args>
bool ThreadPool<R, Args...>::IsWorkerReady(size_t i)
{
	return workers[i].valid() && workers[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

template<class R, class... Args>
ThreadPool<R, Args...>::~ThreadPool()
{
	state = Stopped;
	if (controller.valid())
	{
		controller.wait();
		controller.get();
	}
}

template<class R, class... Args>
void ThreadPool<R, Args...>::AddTask(Task task)
{
	tasks.push(task);
}

template<class R, class... Args>
void ThreadPool<R, Args...>::Stop()
{
	state = Stopped;
}

template<class R, class... Args>
void ThreadPool<R, Args...>::Pause()
{
	state = Paused;
}

template<class R, class... Args>
void ThreadPool<R, Args...>::Continue()
{
	state = Executing;
}

template<class R, class... Args>
bool ThreadPool<R, Args...>::IsAvailable()
{
	return (tasks.size() < workers.size() * tasksPerWorker);
}

template<class R, class... Args>
R ThreadPool<R, Args...>::GetNextResult()
{
	std::lock_guard<std::mutex> lg(resultMutex);
	R res = results.front();
	results.pop();
	return res;
}

template<class R, class... Args>
bool ThreadPool<R, Args...>::CanGetResult()
{
	return !(results.empty());
}

template<class R, class... Args>
void ThreadPool<R, Args...>::Wait()
{
	while (true)
	{
		bool tasksAreComplete = tasks.empty();
		for (size_t i = 0; i < workers.size() && tasksAreComplete; ++i)
		{
			if (workers[i].valid())
				tasksAreComplete = false;
		}
		if (tasksAreComplete) break;
	}
}