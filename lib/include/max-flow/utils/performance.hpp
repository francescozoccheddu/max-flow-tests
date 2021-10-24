#ifndef INCLUDED_MAX_FLOW_UTILS_PERFORMANCE
#define INCLUDED_MAX_FLOW_UTILS_PERFORMANCE

#include <chrono>

namespace MaxFlow::Utils
{

	class Performance final
	{

	private:

		Performance (size_t _ticks, double _time);

		static inline size_t s_ticks;
		static std::chrono::high_resolution_clock::time_point s_startTime;

		size_t m_ticks{};
		double m_time{};

	public:

		Performance () = default;

		size_t ticks () const;
		double time () const;

		inline static constexpr bool enableTicksMeasuring{ true };

		static void start ();

		inline static void tick (size_t _count = 1)
		{
			if constexpr (enableTicksMeasuring)
			{
				s_ticks += _count;
			}
		}

		static Performance end ();

		Performance operator+(const Performance& _other) const;
		Performance operator/(size_t _count) const;
		Performance& operator+=(const Performance& _other);
		Performance& operator/=(size_t _count);

	};

}

#endif