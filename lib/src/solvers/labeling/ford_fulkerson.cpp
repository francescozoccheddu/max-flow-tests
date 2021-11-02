#include <max-flow/solvers/labeling/ford_fulkerson.hpp>


namespace MaxFlow::Solvers::Labeling
{

	void FordFulkersonSolver::solveImpl ()
	{
		do
		{
			calculatePaths (m_depthFirst);
			if (pathfinder ().isSinkLabeled ())
			{
				augmentMax ();
			}
		}
		while (pathfinder ().isSinkLabeled ());
	}


	bool FordFulkersonSolver::depthFirst() const
	{
		return m_depthFirst;
	}

	void FordFulkersonSolver::setDepthFirst(bool _depthFirst)
	{
		m_depthFirst = _depthFirst;
	}

}