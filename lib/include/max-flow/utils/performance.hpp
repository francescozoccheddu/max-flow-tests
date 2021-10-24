#ifndef INCLUDED_MAX_FLOW_UTILS_PERFORMANCE
#define INCLUDED_MAX_FLOW_UTILS_PERFORMANCE

#include <chrono>

namespace MaxFlow::Utils
{

	class Performance final
	{

	private:

		Performance () = default;

		static inline size_t s_ticks;
		static std::chrono::high_resolution_clock::time_point s_startTime;

		size_t m_ticks{};
		double m_time{};

	public:

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

	};

}

#endif