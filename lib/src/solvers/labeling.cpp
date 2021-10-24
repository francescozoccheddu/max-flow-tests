#include <max-flow/solvers/labeling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>
#include <limits>
#include <cmath>
#include <stdexcept>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using namespace MaxFlow::Graphs::Algorithms;
using namespace MaxFlow::Graphs;

namespace MaxFlow::Solvers
{

	const Pathfinder& LabelingSolver::pathfinder () const
	{
		return m_pathfinder;
	}

	Pathfinder& LabelingSolver::pathfinder ()
	{
		return m_pathfinder;
	}

	void LabelingSolver::calculatePaths ()
	{
		m_pathfinder.calculate (edgeSelector ());
	}

	void LabelingSolver::augmentMax ()
	{
		Algorithms::augmentMax (m_pathfinder.begin (), m_pathfinder.end (), areZeroEdgesRemoved ());
		callback ().onAugmentMax (*this);
	}

	const EdgeSelector& LabelingSolver::edgeSelector () const
	{
		return *m_pEdgeSelector;
	}

	EdgeSelector& LabelingSolver::edgeSelector ()
	{
		return *m_pEdgeSelector;
	}

	void LabelingSolver::setEdgeSelector (EdgeSelector& _edgeSelector)
	{
		m_pEdgeSelector = &_edgeSelector;
	}

	const LabelingSolver::Callback& LabelingSolver::callback () const
	{
		return *m_pCallback;
	}

	LabelingSolver::Callback& LabelingSolver::callback ()
	{
		return *m_pCallback;
	}

	void LabelingSolver::setCallback (Callback& _callback)
	{
		m_pCallback = &_callback;
	}

	void LabelingSolver::Callback::onAugmentMax (const LabelingSolver& _solver)
	{}

	LabelingSolver::Callback LabelingSolver::Callback::none{};

	void FordFulkersonSolver::solveImpl ()
	{
		do
		{
			calculatePaths ();
			if (pathfinder ().isSinkLabeled ())
			{
				augmentMax ();
			}
		}
		while (pathfinder ().isSinkLabeled ());
	}

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
			for (const ResidualVertex& vertex : graph ())
			{
				m_distanceCounts[m_distanceLabeler[vertex]]++;
			}
		}
		pathfinder ().reset ();
		pathfinder ().setPredecessor (source (), source ());
		ResidualVertex* pCurrent{ &source () };
		while (m_distanceLabeler[source ()] < graph ().verticesCount ())
		{
			const size_t distance{ m_distanceLabeler[*pCurrent] };
			bool foundAdmissibleEdge{ false };
			for (ResidualEdge& edge : *pCurrent)
			{
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