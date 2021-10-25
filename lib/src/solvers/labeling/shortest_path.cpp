#include <max-flow/solvers/labeling/shortest_path.hpp>

#include <max-flow/utils/performance.hpp>
#include <limits>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;

namespace MaxFlow::Solvers::Labeling
{

	bool ShortestPathSolver::isMinCutDetectionEnabled () const
	{
		return m_detectMinCut;
	}

	void ShortestPathSolver::setMinCutDetection (bool _enabled)
	{
		m_detectMinCut = _enabled;
	}

	void ShortestPathSolver::solveImpl ()
	{
		m_distanceLabeler.calculate (edgeSelector ());
		if (isMinCutDetectionEnabled ())
		{
			m_distanceCounts.clear ();
			m_distanceCounts.resize (graph ().verticesCount (), 0);
			Utils::Performance::tick (graph ().verticesCount ());
			for (const ResidualVertex& vertex : graph ())
			{
				const size_t distance{ m_distanceLabeler[vertex] };
				if (distance < m_distanceCounts.size ())
				{
					m_distanceCounts[distance]++;
				}
			}
		}
		pathfinder ().reset ();
		pathfinder ().setPredecessor (source (), source ());
		ResidualVertex* pCurrent{ &source () };
		while (m_distanceLabeler[source ()] < graph ().verticesCount ())
		{
			Utils::Performance::tick ();
			const size_t distance{ m_distanceLabeler[*pCurrent] };
			bool foundAdmissibleEdge{ false };
			for (ResidualEdge& edge : *pCurrent)
			{
				Utils::Performance::tick ();
				if (m_distanceLabeler.isAdmissible (edge) && edgeSelector ()(edge))
				{
					pathfinder ().setPredecessor (edge);
					pCurrent = &edge.to ();
					if (pCurrent == &sink ())
					{
						augmentMax ();
						pCurrent = &source ();
					}
					foundAdmissibleEdge = true;
					break;
				}
			}
			if (!foundAdmissibleEdge)
			{
				size_t minDistance{ std::numeric_limits<size_t>::max () };
				bool hasOutEdges{};
				for (ResidualEdge& edge : *pCurrent)
				{
					Utils::Performance::tick ();
					if (*edge && m_distanceLabeler[edge.to ()] < minDistance && edgeSelector () (edge))
					{
						hasOutEdges = true;
						minDistance = m_distanceLabeler[edge.to ()];
					}
				}
				if (!hasOutEdges)
				{
					break;
				}
				m_distanceLabeler.setDistance (*pCurrent, minDistance + 1);
				pCurrent = &pathfinder ()[*pCurrent];
				if (isMinCutDetectionEnabled ())
				{
					m_distanceCounts[distance]--;
					m_distanceCounts[minDistance + 1]++;
					if (!m_distanceCounts[distance])
					{
						break;
					}
				}
			}
		}
	}

}