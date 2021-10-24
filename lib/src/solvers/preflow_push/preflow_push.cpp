#include <max-flow/solvers/preflow_push/preflow_push.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <limits>
#include <max-flow/utils/performance.hpp>
#include <algorithm>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;

namespace MaxFlow::Solvers::PreflowPush
{

	bool PreflowPushSolver::Excess::isExcess () const
	{
		return pVertex && amount;
	}

	void PreflowPushSolver::solveImpl ()
	{
		m_distanceLabeler.calculate ();
		m_distanceLabeler.setDistance (source (), graph ().verticesCount ());
		initialize ();
		for (ResidualEdge& edge : source ())
		{
			Utils::Performance::tick ();
			const flow_t amount{ *edge };
			Graphs::Algorithms::augment (edge, amount, areZeroEdgesRemoved ());
			addExcess (edge, amount);
		}
		Excess excess{ getExcess () };
		while (excess.isExcess ())
		{
			Utils::Performance::tick ();
			bool foundAdmissibleEdge{};
			for (ResidualEdge& edge : *excess.pVertex)
			{
				Utils::Performance::tick ();
				if (m_distanceLabeler.isAdmissible (edge))
				{
					const flow_t amount{ std::min (*edge, std::min (excess.amount, maximumPushAmount (edge, excess))) };
					Graphs::Algorithms::augment (edge, amount, areZeroEdgesRemoved ());
					addExcess (edge, amount);
					foundAdmissibleEdge = true;
					break;
				}
			}
			if (!foundAdmissibleEdge)
			{
				size_t minDistance{ std::numeric_limits<size_t>::max () };
				bool hasOutEdges{};
				for (ResidualEdge& edge : *excess.pVertex)
				{
					Utils::Performance::tick ();
					if (*edge && m_distanceLabeler[edge.to ()] < minDistance)
					{
						hasOutEdges = true;
						minDistance = m_distanceLabeler[edge.to ()];
					}
				}
				if (!hasOutEdges)
				{
					break;
				}
				const size_t oldDistance{ m_distanceLabeler[*excess.pVertex] };
				m_distanceLabeler.setDistance (*excess.pVertex, minDistance + 1);
				onRelabel (*excess.pVertex, oldDistance);
			}
			excess = getExcess ();
		}
	}

	size_t Solvers::PreflowPush::PreflowPushSolver::distance (const ResidualVertex& _vertex) const
	{
		return m_distanceLabeler[_vertex];
	}

	void PreflowPushSolver::onRelabel (ResidualVertex& _vertex, size_t _oldLabel)
	{}

	flow_t PreflowPushSolver::maximumPushAmount (const ResidualEdge& _edge, Excess _fromExcess) const
	{
		return std::numeric_limits<flow_t>::max ();
	}

}