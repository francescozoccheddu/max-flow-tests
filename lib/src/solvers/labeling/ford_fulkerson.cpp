#include <max-flow/solvers/labeling/ford_fulkerson.hpp>

#include <max-flow/utils/performance.hpp>

namespace MaxFlow::Solvers::Labeling
{

	void FordFulkersonSolver::solveImpl ()
	{
		do
		{
			Utils::Performance::tick ();
			calculatePaths ();
			if (pathfinder ().isSinkLabeled ())
			{
				augmentMax ();
			}
		}
		while (pathfinder ().isSinkLabeled ());
	}

}