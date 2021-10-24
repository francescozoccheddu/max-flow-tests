#include <max-flow/solvers/preflow_push/preflow_push.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <limits>
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
			const flow_t amount{ *edge };
			Graphs::Algorithms::augment (edge, amount, areZeroEdgesRemoved ());
			addExcess (edge, amount);
		}
		Excess excess{ getExcess () };
		while (excess.isExcess ())
		{
			bool foundAdmissibleEdge{};
			for (ResidualEdge& edge : *excess.pVertex)
			{
				if (m_distanceLabeler.isAdmissible (edge))
				{
					const flow_t amount{ std::min (*edge, excess.amount) };
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
				m_distanceLabeler.setDistance (*excess.pVertex, minDistance + 1);
			}
			excess = getExcess ();
		}
	}

}