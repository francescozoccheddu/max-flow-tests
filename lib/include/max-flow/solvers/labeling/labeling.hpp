#ifndef INCLUDED_MAX_FLOW_SOLVERS_LABELING_LABELING
#define INCLUDED_MAX_FLOW_SOLVERS_LABELING_LABELING

#include <max-flow/solve.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>
#include <max-flow/graphs/algorithms/edge_selector.hpp>

namespace MaxFlow::Solvers::Labeling
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

		const Graphs::Algorithms::EdgeSelector& edgeSelector () const;
		Graphs::Algorithms::EdgeSelector& edgeSelector ();
		void setEdgeSelector (Graphs::Algorithms::EdgeSelector& _edgeSelector);

		const Callback& callback () const;
		Callback& callback ();
		void setCallback (Callback& _callback);

		using Solver::Solver;

	};

}

#endif