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

	public:

		class Callback
		{

			friend class LabelingSolver;

			virtual void onAugmentMax (const LabelingSolver& _solver);

		public:

			static Callback none;

		};

	private:

		Callback* m_pCallback{&Callback::none};
		Graphs::Algorithms::Pathfinder m_pathfinder{ graph (), source (), sink () };
		Graphs::Algorithms::EdgeSelector* m_pEdgeSelector{ &Graphs::Algorithms::EdgeSelector::all };

	protected:

		Graphs::Algorithms::Pathfinder& pathfinder ();
		void calculatePaths ();
		void augmentMax ();

	public:

		const Graphs::Algorithms::Pathfinder& pathfinder () const;

		const Graphs::Algorithms::EdgeSelector& const edgeSelector () const;
		Graphs::Algorithms::EdgeSelector& edgeSelector ();
		void setEdgeSelector (Graphs::Algorithms::EdgeSelector& _edgeSelector);

		const Callback& callback () const;
		Callback& callback ();
		void setCallback (Callback& _callback);

		using Solver::Solver;

	};

	class FordFulkersonSolver final : public LabelingSolver
	{

	private:

		void solveImpl () override;

	public:

		using LabelingSolver::LabelingSolver;

	};

	class ShortestPathSolver final : public LabelingSolver
	{

	private:

		Graphs::Algorithms::DistanceLabeler m_distanceLabeler{ graph (), source (), sink () };
		std::vector<size_t> m_distanceCounts{};
		bool m_detectMinCut{};

		void solveImpl () override;

	public:

		bool isMinCutDetectionEnabled () const;
		void setMinCutDetection (bool _enabled);

		using LabelingSolver::LabelingSolver;

	};

}

#endif