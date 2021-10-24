#include <max-flow/utils/performance.hpp>

namespace MaxFlow::Utils
{

	std::chrono::high_resolution_clock::time_point Performance::s_startTime;

	size_t Performance::ticks () const
	{
		return m_ticks;
	}

	double Performance::time () const
	{
		return m_time;
	}

	void Performance::start ()
	{
		s_startTime = std::chrono::high_resolution_clock::now ();
		s_ticks = 0;
	}

	Performance Performance::end ()
	{
		const std::chrono::high_resolution_clock::time_point endTime{ std::chrono::high_resolution_clock::now () };
		const double time{ std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - s_startTime).count () / 1000000000.0 };
		Performance performance;
		performance.m_ticks = s_ticks;
		performance.m_time = time;
		return performance;
	}

}