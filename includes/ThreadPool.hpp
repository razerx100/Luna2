#ifndef THREAD_POOL_STANDARD_HPP_
#define THREAD_POOL_STANDARD_HPP_
#include <condition_variable>
#include <functional>
#include <queue>

class ThreadPool {
public:
	ThreadPool(size_t threadCount);
	~ThreadPool() noexcept;

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	void SubmitWork(std::function<void()> workFunction) noexcept;

private:
	void WorkThread();

private:
	std::atomic_bool m_worksDone;
	std::atomic_size_t m_threadRunningCounter;
	std::atomic_size_t m_worksCount;
	std::vector<std::thread> m_threads;
	std::queue<std::function<void()>> m_workQueue;
	std::mutex m_mutex;
	std::condition_variable m_cVar;
};
#endif
