#include <max-flow/solvers/preflow_push/highest_label.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <algorithm>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;

namespace MaxFlow::Solvers::PreflowPush
{

	void HighestLabelPreflowPushSolver::initialize ()
	{
		m_maxDistance = graph ().verticesCount () - 1;
		m_excesses.clear ();
		m_excesses.resize (graph ().verticesCount (), 0);
		m_activeVerticesPerDistance.clear ();
		m_activeVerticesPerDistance.resize (graph ().verticesCount (), {});
	}

	void HighestLabelPreflowPushSolver::addExcess (ResidualEdge& _edge, flow_t _amount)
	{
		if (_edge.to () != source () && _edge.to () != sink ())
		{
			if (!m_excesses[_edge.to ().index ()])
			{
				push (_edge.to ());
			}
			m_excesses[_edge.to ().index ()] += _amount;
		}
		if (_edge.from () != source () && _edge.from () != sink ())
		{
			m_excesses[_edge.from ().index ()] -= _amount;
		}
	}

	PreflowPushSolver::Excess HighestLabelPreflowPushSolver::getExcess ()
	{
		while (m_maxDistance > 0)
		{
			while (!m_activeVerticesPerDistance[m_maxDistance].empty ())
			{
				ResidualVertex& vertex{ *m_activeVerticesPerDistance[m_maxDistance].front () };
				if (m_excesses[vertex.index ()] && distance (vertex) == m_maxDistance)
				{
					return { .pVertex{&vertex}, .amount{m_excesses[vertex.index ()]} };
				}
				m_activeVerticesPerDistance[m_maxDistance].pop ();
			}
			m_maxDistance--;
		}
		return {};
	}

	void HighestLabelPreflowPushSolver::onRelabel (Graphs::ResidualVertex& _vertex, size_t _oldDistance)
	{
		push (_vertex);
	}

	void HighestLabelPreflowPushSolver::push (Graphs::ResidualVertex& _vertex)
	{
		const size_t newDistance{ distance (_vertex) };
		if (newDistance > m_maxDistance)
		{
			m_maxDistance = newDistance;
		}
		m_activeVerticesPerDistance[newDistance].push (&_vertex);
	}

}