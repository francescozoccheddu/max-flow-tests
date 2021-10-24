#ifndef INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_PREFLOW_PUSH
#define INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_PREFLOW_PUSH

#include <max-flow/solve.hpp>
#include <max-flow/graphs/algorithms/distance_labeler.hpp>

namespace MaxFlow::Solvers::PreflowPush
{

	class PreflowPushSolver : public Solver
	{

	private:

		Graphs::Algorithms::DistanceLabeler m_distanceLabeler{ graph (), source (), sink () };

		void solveImpl () override final;

	protected:

		struct Excess
		{

			Graphs::ResidualVertex* pVertex{};
			Graphs::flow_t amount{};

			bool isExcess () const;

		};

		virtual void initialize () = 0;
		virtual void addExcess (Graphs::ResidualEdge& _edge, Graphs::flow_t _amount) = 0;
		virtual Excess getExcess () = 0;
		virtual void onRelabel (Graphs::ResidualVertex& _vertex, size_t _oldLabel);
		size_t distance (const Graphs::ResidualVertex& _vertex) const;

	public:

		using Solver::Solver;

	};

}

#endif