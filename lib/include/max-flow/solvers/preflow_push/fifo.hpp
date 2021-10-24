#ifndef INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_FIFO
#define INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_FIFO

#include <max-flow/solvers/preflow_push/preflow_push.hpp>
#include <queue>
#include <vector>

namespace MaxFlow::Solvers::PreflowPush
{

	class FifoPreflowPushSolver final : public PreflowPushSolver
	{

	private:

		std::vector<Graphs::flow_t> m_excesses{};
		std::queue<Graphs::ResidualVertex*> m_activeVertices{};

		void initialize () override;
		void addExcess (Graphs::ResidualEdge& _edge, Graphs::flow_t _amount) override;
		Excess getExcess () override;

	public:

		using PreflowPushSolver::PreflowPushSolver;

	};

}

#endif