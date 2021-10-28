#include <max-flow/solvers/labeling/ford_fulkerson.hpp>


namespace MaxFlow::Solvers::Labeling
{

	void FordFulkersonSolver::solveImpl ()
	{
		do
		{
			calculatePaths ();
			if (pathfinder ().isSinkLabeled ())
			{
				augmentMax ();
			}
		}
		while (pathfinder ().isSinkLabeled ());
	}

}