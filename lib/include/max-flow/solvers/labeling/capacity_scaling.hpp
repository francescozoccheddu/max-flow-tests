#ifndef INCLUDED_MAX_FLOW_SOLVERS_LABELING_CAPACITY_SCALING
#define INCLUDED_MAX_FLOW_SOLVERS_LABELING_CAPACITY_SCALING

#include <max-flow/solve.hpp>
#include <max-flow/solvers/labeling/labeling.hpp>
#include <max-flow/graphs/algorithms/edge_selector.hpp>

namespace MaxFlow::Solvers::Labeling
{

	class CapacityScalingSolver final : public Solver
	{

	public:

		enum class ESubSolver
		{
			FordFulkerson, ShortestPath
		};

	private:

		struct PropagateCallback final : public LabelingSolver::Callback
		{

			Graphs::ResidualGraph& m_graph;

			void onAugmentMax (const LabelingSolver& _solver);

		public:

			PropagateCallback (Graphs::ResidualGraph& _graph);

		};

		struct DeltaEdgeSelector final : public Graphs::Algorithms::EdgeSelector
		{

			Graphs::flow_t delta{};

			bool operator() (const Graphs::ResidualEdge& _edge) override;

		};

		ESubSolver m_subSolver{ ESubSolver::FordFulkerson};
		bool m_removeDeltaEdges{};

		void solveImpl () override;

	public:

		bool areDeltaEdgesRemoved () const;
		void setRemoveDeltaEdges (bool _enabled);

		ESubSolver subSolver () const;
		void setSubSolver (ESubSolver _subSolver);

		using Solver::Solver;

	};

}

#endif