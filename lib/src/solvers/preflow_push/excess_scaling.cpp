#include <max-flow/solvers/preflow_push/excess_scaling.hpp>

#include <cmath>
#include <algorithm>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;

namespace MaxFlow::Solvers::PreflowPush
{

	void ExcessScalingPreflowPushSolver::initialize ()
	{
		m_minDistance = 1;
		m_excesses.clear ();
		m_excesses.resize (graph ().verticesCount (), 0);
		m_activeVerticesPerDistance.clear ();
		m_activeVerticesPerDistance.resize (2 * graph ().verticesCount () - 1, {});
		flow_t maxCapacity{ 0 };
		for (const ResidualVertex& vertex : graph ())
		{
			for (const ResidualEdge& edge : vertex)
			{
				if (capacities ()[edge] > maxCapacity)
				{
					maxCapacity = capacities ()[edge];
				}
			}
		}
		m_delta = static_cast<flow_t>(std::pow (2, std::ceil (std::log2 (maxCapacity))));
	}

	void ExcessScalingPreflowPushSolver::addExcess (ResidualEdge& _edge, flow_t _amount)
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

	PreflowPushSolver::Excess ExcessScalingPreflowPushSolver::getExcess ()
	{
		while (m_delta >= 1)
		{
			while (m_minDistance < m_activeVerticesPerDistance.size())
			{
				while (!m_activeVerticesPerDistance[m_minDistance].empty ())
				{
					ResidualVertex& vertex{ *m_activeVerticesPerDistance[m_minDistance].front () };
					if (m_excesses[vertex.index ()] && label (vertex) == m_minDistance)
					{
						return { .pVertex{&vertex}, .amount{m_excesses[vertex.index ()]} };
					}
					m_activeVerticesPerDistance[m_minDistance].pop ();
				}
				m_minDistance++;
			}
			m_delta /= 2;
			updateActiveNodes ();
		}
		return {};
	}

	void ExcessScalingPreflowPushSolver::onRelabel (Graphs::ResidualVertex& _vertex, size_t _oldDistance)
	{
		push (_vertex);
	}

	void ExcessScalingPreflowPushSolver::push (Graphs::ResidualVertex& _vertex)
	{
		const size_t newDistance{ *label (_vertex) };
		if (newDistance < m_minDistance)
		{
			m_minDistance = newDistance;
		}
		m_activeVerticesPerDistance[newDistance].push (&_vertex);
	}

	flow_t ExcessScalingPreflowPushSolver::maximumPushAmount (const Graphs::ResidualEdge& _edge, Excess _fromExcess) const
	{
		return m_delta - m_excesses[_edge.to ().index ()];
	}

	void ExcessScalingPreflowPushSolver::updateActiveNodes ()
	{
		for (size_t i{ 0 }; i < m_activeVerticesPerDistance.size (); i++)
		{
			m_activeVerticesPerDistance[i] = {};
		}
		for (ResidualVertex& vertex : graph ())
		{
			if (m_excesses[vertex.index ()] > m_delta / 2)
			{
				push (vertex);
			}
		}
	}

}