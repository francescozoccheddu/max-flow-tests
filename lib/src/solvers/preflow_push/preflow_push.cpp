#include <max-flow/solvers/preflow_push/preflow_push.hpp>

#include <limits>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;

namespace MaxFlow::Solvers::PreflowPush
{

	void PreflowPushSolver::solveImpl ()
	{
		m_distanceLabeler.calculate ();
		m_distanceLabeler.setDistance (source (), graph ().verticesCount ());
		initialize ();
		for (ResidualEdge& edge : source ())
		{
			augment (edge);
		}
		ResidualVertex* pActiveVertex{ popActiveVertexIfExists () };
		while (pActiveVertex)
		{
			ResidualVertex& vertex{ *pActiveVertex };
			bool foundAdmissibleEdge{};
			for (ResidualEdge& edge : vertex)
			{
				if (m_distanceLabeler.isAdmissible (edge))
				{
					augment (edge);
					foundAdmissibleEdge = true;
					break;
				}
			}
			if (!foundAdmissibleEdge)
			{
				size_t minDistance{ std::numeric_limits<size_t>::max () };
				bool hasOutEdges{};
				for (ResidualEdge& edge : vertex)
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
				m_distanceLabeler.setDistance (vertex, minDistance + 1);
			}
			pActiveVertex = popActiveVertexIfExists ();
		}
	}

}