#include <max-flow-app/performance.hpp>

#include <cmath>

namespace MaxFlow::App
{

	std::chrono::high_resolution_clock::time_point Performance::s_startTime;

	void Performance::start ()
	{
		s_startTime = std::chrono::high_resolution_clock::now ();
	}

	double Performance::end ()
	{
		const std::chrono::high_resolution_clock::time_point endTime{ std::chrono::high_resolution_clock::now () };
		return  std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - s_startTime).count () / 1000000000.0 ;
	}


}

