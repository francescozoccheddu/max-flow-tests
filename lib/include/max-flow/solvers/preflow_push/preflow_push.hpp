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

		virtual void initialize () = 0;
		virtual void augment (Graphs::ResidualEdge& _edge) = 0;
		virtual Graphs::ResidualVertex* popActiveVertexIfExists () = 0;

	public:

		using Solver::Solver;

	};

}

#endif