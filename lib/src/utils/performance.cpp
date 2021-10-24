#include <max-flow/utils/performance.hpp>

#include <cmath>

namespace MaxFlow::Utils
{

	std::chrono::high_resolution_clock::time_point Performance::s_startTime;

	Performance::Performance (size_t _ticks, double _time) : m_ticks{ _ticks }, m_time{ _time }
	{}

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
		return { s_ticks, time };
	}

	Performance Performance::operator+(const Performance& _other) const
	{
		return Performance{ *this } += _other;
	}

	Performance Performance::operator/(size_t _count) const
	{
		return Performance{ *this } /= _count;
	}

	Performance& Performance::operator+=(const Performance& _other)
	{
		m_ticks += _other.m_ticks;
		m_time += _other.m_time;
		return *this;
	}

	Performance& Performance::operator/=(size_t _count)
	{
		m_ticks = static_cast<size_t>(std::round (m_ticks / static_cast<double>(_count)));
		m_time /= _count;
		return *this;
	}


}

