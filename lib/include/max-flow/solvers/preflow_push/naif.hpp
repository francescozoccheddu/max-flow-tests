#ifndef INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_NAIF
#define INCLUDED_MAX_FLOW_SOLVERS_PREFLOW_PUSH_NAIF

#include <max-flow/solvers/preflow_push/preflow_push.hpp>
#include <vector>

namespace MaxFlow::Solvers::PreflowPush
{

	class NaifPreflowPushSolver final : public PreflowPushSolver
	{

	private:

		std::vector<Graphs::flow_t> m_excesses{};

		void initialize () override;
		void augment (Graphs::ResidualEdge& _edge) override;
		Graphs::ResidualVertex* popActiveVertexIfExists () override;

	public:

		using PreflowPushSolver::PreflowPushSolver;

	};

}

#endif