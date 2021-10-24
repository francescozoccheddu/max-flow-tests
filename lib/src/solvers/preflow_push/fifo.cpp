#include <max-flow/solvers/preflow_push/fifo.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <algorithm>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;

namespace MaxFlow::Solvers::PreflowPush
{

	void FifoPreflowPushSolver::initialize ()
	{
		m_excesses.clear ();
		m_excesses.resize (graph ().verticesCount (), 0);
		m_activeVertices = {};
	}

	void FifoPreflowPushSolver::addExcess (ResidualEdge& _edge, flow_t _amount)
	{
		if (_edge.to () != source () && _edge.to () != sink ())
		{
			if (!m_excesses[_edge.to ().index ()])
			{
				m_activeVertices.push (&_edge.to ());
			}
			m_excesses[_edge.to ().index ()] += _amount;
		}
		if (_edge.from () != source () && _edge.from () != sink ())
		{
			m_excesses[_edge.from ().index ()] -= _amount;
		}
	}

	PreflowPushSolver::Excess FifoPreflowPushSolver::getExcess ()
	{
		while (!m_activeVertices.empty ())
		{
			ResidualVertex& vertex{ *m_activeVertices.front () };
			if (m_excesses[vertex.index ()])
			{
				return { .pVertex{&vertex}, .amount{m_excesses[vertex.index ()]} };
			}
			m_activeVertices.pop ();
		}
		return {};
	}

}