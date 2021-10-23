#ifndef INCLUDED_MAX_FLOW_SOLVERS_LABELING
#define INCLUDED_MAX_FLOW_SOLVERS_LABELING

#include <max-flow/solve.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>
#include <max-flow/graphs/algorithms/distance_labeler.hpp>
#include <max-flow/graphs/algorithms/edge_selector.hpp>
#include <vector>

namespace MaxFlow::Solvers
{

	class LabelingSolver : public Solver
	{

	private:

		void solveImpl () override;

	public:

		using Solver::Solver;

		virtual void solveWithEdgeSelector (Graphs::Algorithms::EdgeSelector& _edgeSelector) = 0;

	};

	class FordFulkersonSolver final : public LabelingSolver
	{

	private:

		Graphs::Algorithms::Pathfinder m_pathfinder{ graph (), source (), sink () };

		void solveWithEdgeSelector (Graphs::Algorithms::EdgeSelector& _edgeSelector) override;

	public:

		using LabelingSolver::LabelingSolver;

	};

	class ShortestPathSolver final : public LabelingSolver
	{

	private:

		Graphs::Algorithms::Pathfinder m_pathfinder{ graph (), source (), sink () };
		Graphs::Algorithms::DistanceLabeler m_distanceLabeler{ graph (), source (), sink () };
		std::vector<size_t> m_distanceCounts{};
		bool m_detectMinCut{};

		void solveWithEdgeSelector (Graphs::Algorithms::EdgeSelector& _edgeSelector) override;

	public:

		bool isMinCutDetectionEnabled () const;
		void setMinCutDetection (bool _enabled);

		using LabelingSolver::LabelingSolver;

	};

	class CapacityScalingSolver final : public Solver
	{

	public:

		enum class ESubSolver
		{
			FordFulkerson, ShortestPath
		};

	private:


		struct DeltaEdgeSelector : public Graphs::Algorithms::EdgeSelector
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
		void setSubSolver(ESubSolver _subSolver);

		using Solver::Solver;

	};

}

#endif