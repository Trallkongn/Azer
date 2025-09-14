#pragma once

#include <vector>
#include <thread>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <type_traits>
#include <utility>

namespace Azer {
	class ThreadPool {
	public:
		explicit ThreadPool(size_t worker_count = std::thread::hardware_concurrency())
			: stop_flag(false)
		{
			if (worker_count == 0) worker_count = 1;
			workers.reserve(worker_count);
			for (size_t i = 0; i < worker_count; ++i) {
				workers.emplace_back([this]() {this->worker_loop(); });
			}
		}

		~ThreadPool() {
			shutdown();
		}

		// Submit a task. Returns std::future<R> where R is the callable's return type.
		template<class F, class...Args>
		auto submit(F&& f, Args&&... args)
			-> std::future<typename std::invoke_result<F,Args...>::type>
		{
			using return_type = typename std::invoke_result<F, Args...>::type;

			// wrap the task in a packaged_task
			auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
				std::bind(std::forward<F>(f),std::forward<Args>(args)...)
			);

			std::future<return_type> res = task_ptr->get_future();

			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				if(stop_flag.load()) 
					throw std::runtime_error("submit on stopped ThreadPool");

				tasks.emplace([task_ptr]() { (*task_ptr)(); });
			}
			cond_var.notify_one();
			return res;
		}

		// Graceful shutdown: stop accepting new tasks, finish queued tasks, join threads
		void shutdown() {
			bool expected = false;
			// ensure shutdown only runs once
			if (!stop_flag.compare_exchange_strong(expected, true)) return;

			cond_var.notify_all();
			for (std::thread& worker : workers) {
				if(worker.joinable()) worker.join();
			}
		}

		// number of worker threads
		size_t GetWorkerCount() const {
			return workers.size();
		}
	private:
		// worker thread function
		void worker_loop() {
			while (true) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(queue_mutex);
					cond_var.wait(lock, [this]() {return stop_flag.load() || !tasks.empty(); });
					if (stop_flag.load() && tasks.empty()) return;
					task = std::move(tasks.front());
					tasks.pop();
				}
				// excute outside lock
				try {
					task();
				}
				catch (...) {
					// swallow exceptions: they should be handled inside the task via futures
				}
			}
		}

		std::vector<std::thread> workers;
		std::queue<std::function<void()>> tasks;

		std::mutex queue_mutex;
		std::condition_variable cond_var;
		std::atomic<bool> stop_flag;
	};
}