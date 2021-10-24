#ifndef INCLUDED_MAX_FLOW_SOLVERS_CAPACITY_SCALING
#define INCLUDED_MAX_FLOW_SOLVERS_CAPACITY_SCALING

#include <max-flow/solve.hpp>
#include <max-flow/solvers/labeling.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>
#include <max-flow/graphs/algorithms/distance_labeler.hpp>
#include <max-flow/graphs/algorithms/edge_selector.hpp>
#include <vector>

namespace MaxFlow::Solvers
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

		ESubSolver m_subSolver;
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