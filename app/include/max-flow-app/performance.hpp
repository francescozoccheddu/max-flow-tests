#ifndef INCLUDED_MAX_FLOW_APP_PERFORMANCE
#define INCLUDED_MAX_FLOW_APP_PERFORMANCE

#include <chrono>

namespace MaxFlow::Utils
{

	class Performance final
	{

	private:

		Performance () = delete;
		~Performance () = delete;

		static std::chrono::high_resolution_clock::time_point s_startTime;

	public:

		static void start ();

		static double end ();

	};

}

#endif