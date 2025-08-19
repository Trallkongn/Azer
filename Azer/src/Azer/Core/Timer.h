#pragma once
#include <iostream>
#include <chrono>

namespace Azer {

	template<typename Fn>
	class Timer
	{
	public:
		Timer(const char* name, Fn&& func)
			: m_Name(name), m_Stopped(false), m_Func(func)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if (!m_Stopped) Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;

			float duration = (end - start) * 0.001f;

			std::cout << m_Name << " Duration: " << duration << " ms " << std::endl;

			m_Func({ m_Name, duration });
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;

		Fn m_Func;
	};
}