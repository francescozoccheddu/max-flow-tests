#ifndef INCLUDED_MAX_FLOW_SOLVERS_LABELING_FORD_FULKERSON
#define INCLUDED_MAX_FLOW_SOLVERS_LABELING_FORD_FULKERSON

#include <max-flow/solvers/labeling/labeling.hpp>

namespace MaxFlow::Solvers::Labeling
{

	class FordFulkersonSolver final : public LabelingSolver
	{

	private:

		bool m_depthFirst{false};

		void solveImpl () override;

	public:

		bool depthFirst() const;
		void setDepthFirst(bool _depthFirst);

		using LabelingSolver::LabelingSolver;

	};

}

#endif