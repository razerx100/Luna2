#include <ThreadPool.hpp>

ThreadPool::ThreadPool(size_t threadCount)
	: m_worksDone(false), m_threadRunningCounter(0u), m_worksCount(0u) {

	for (size_t index = 0u; index < threadCount; ++index)
		m_threads.emplace_back(&ThreadPool::WorkThread, this);
}

ThreadPool::~ThreadPool() noexcept {
	while (m_threadRunningCounter != 0u && m_worksCount != 0u);
	m_worksDone = true;
	m_cVar.notify_all();

	for (std::thread& thread : m_threads)
		thread.join();
}

void ThreadPool::SubmitWork(std::function<void()> workFunction) noexcept {
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_workQueue.push(workFunction);
		++m_worksCount;
	}

	m_cVar.notify_one();
}

void ThreadPool::WorkThread() {
	while (true) {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cVar.wait(lock, [&] { return !m_workQueue.empty() || m_worksDone; });

		if (!m_worksDone) {
			std::function<void()> work = m_workQueue.front();
			m_workQueue.pop();
			lock.unlock();

			++m_threadRunningCounter;
			work();
			--m_threadRunningCounter;
			--m_worksCount;
		}
		else
			break;
	}
}
