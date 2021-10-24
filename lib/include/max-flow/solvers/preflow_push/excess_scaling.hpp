#ifndef INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_EXCESS_SCALING
#define INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_EXCESS_SCALING

#include <max-flow/solvers/preflow_push/preflow_push.hpp>
#include <queue>
#include <vector>

namespace MaxFlow::Solvers::PreflowPush
{

	class ExcessScalingPreflowPushSolver final : public PreflowPushSolver
	{

	private:

		std::vector<Graphs::flow_t> m_excesses{};
		std::vector<std::queue<Graphs::ResidualVertex*>> m_activeVerticesPerDistance{};
		size_t m_maxDistance{};

		void initialize () override;
		void addExcess (Graphs::ResidualEdge& _edge, Graphs::flow_t _amount) override;
		Excess getExcess () override;
		void onRelabel(Graphs::ResidualVertex& _vertex, size_t _oldDistance) override;
		void push (Graphs::ResidualVertex& _vertex);

	public:

		using PreflowPushSolver::PreflowPushSolver;

	};

}

#endif