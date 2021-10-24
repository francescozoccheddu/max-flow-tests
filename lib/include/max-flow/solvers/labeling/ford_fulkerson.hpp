#ifndef INCLUDED_MAX_FLOW_SOLVERS_LABELING_FORD_FULKERSON
#define INCLUDED_MAX_FLOW_SOLVERS_LABELING_FORD_FULKERSON

#include <max-flow/solvers/labeling/labeling.hpp>

namespace MaxFlow::Solvers::Labeling
{

	class FordFulkersonSolver final : public LabelingSolver
	{

	private:

		void solveImpl () override;

	public:

		using LabelingSolver::LabelingSolver;

	};

}

#endif