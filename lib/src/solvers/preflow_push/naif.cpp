#include <max-flow/solvers/preflow_push/naif.hpp>


using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;


namespace MaxFlow::Solvers::PreflowPush
{

	void NaifPreflowPushSolver::initialize ()
	{
		m_excesses.clear ();
		m_excesses.resize (graph ().verticesCount (), 0);
	}

	void NaifPreflowPushSolver::addExcess (ResidualEdge& _edge, flow_t _amount)
	{
		if (_edge.to () != source () && _edge.to () != sink ())
		{
			m_excesses[_edge.to ().index ()] += _amount;
		}
		if (_edge.from () != source () && _edge.from () != sink ())
		{
			m_excesses[_edge.from ().index ()] -= _amount;
		}
	}

	PreflowPushSolver::Excess NaifPreflowPushSolver::getExcess ()
	{
		for (size_t i{ 0 }; i < graph ().verticesCount (); i++)
		{
			if (m_excesses[i])
			{
				return { .pVertex{&graph ()[i]}, .amount{m_excesses[i]} };
			}
		}
		return {};
	}

}