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

	void LabelingSolver::solveImpl ()
	{
		EdgeSelector edgeSelector;
		solveWithEdgeSelector (edgeSelector);
	}

	void FordFulkersonSolver::solveWithEdgeSelector (EdgeSelector& _edgeSelector)
	{
		do
		{
			m_pathfinder.calculate (_edgeSelector);
			if (m_pathfinder.isSinkLabeled ())
			{
				augmentMax (m_pathfinder.begin (), m_pathfinder.end (), areZeroEdgesRemoved ());
			}
		}
		while (m_pathfinder.isSinkLabeled ());
	}

	bool ShortestPathSolver::isMinCutDetectionEnabled () const
	{
		return m_detectMinCut;
	}

	void ShortestPathSolver::setMinCutDetection (bool _enabled)
	{
		m_detectMinCut = _enabled;
	}

	void ShortestPathSolver::solveWithEdgeSelector (EdgeSelector& _edgeSelector)
	{
		m_distanceLabeler.calculate (_edgeSelector);
		if (isMinCutDetectionEnabled())
		{
			m_distanceCounts.clear ();
			m_distanceCounts.resize (graph ().verticesCount (), 0);
			for (const ResidualVertex& vertex : graph())
			{
				m_distanceCounts[m_distanceLabeler[vertex]]++;
			}
		}
		m_pathfinder.reset ();
		m_pathfinder.setPredecessor (source(), source ());
		ResidualVertex* pCurrent{ &source () };
		while (m_distanceLabeler[source ()] < graph ().verticesCount ())
		{
			const size_t distance{ m_distanceLabeler[*pCurrent] };
			bool foundAdmissibleEdge{ false };
			for (ResidualEdge& edge : *pCurrent)
			{
				if (m_distanceLabeler.isAdmissible (edge) && _edgeSelector(edge))
				{
					m_pathfinder.setPredecessor (edge);
					pCurrent = &edge.to ();
					if (pCurrent == &sink())
					{
						augmentMax (m_pathfinder.begin (), m_pathfinder.end (), areZeroEdgesRemoved());
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
					if (*edge && m_distanceLabeler[edge.to ()] < minDistance && _edgeSelector (edge))
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
				pCurrent = &m_pathfinder[*pCurrent];
				if (isMinCutDetectionEnabled())
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

	bool CapacityScalingSolver::areDeltaEdgesRemoved () const
	{
		return m_removeDeltaEdges;
	}

	void CapacityScalingSolver::setRemoveDeltaEdges (bool _enabled)
	{
		m_removeDeltaEdges = _enabled;
	}

	CapacityScalingSolver::ESubSolver CapacityScalingSolver::subSolver () const
	{
		return m_subSolver;
	}


	void CapacityScalingSolver::setSubSolver (ESubSolver _subSolver)
	{
		m_subSolver = _subSolver;
	}

	void CapacityScalingSolver::solveImpl ()
	{
		ResidualGraph deltaGraphStorage;
		if (areDeltaEdgesRemoved())
		{
			deltaGraphStorage.setMatrix (true);
			deltaGraphStorage.addVertices (graph().verticesCount ());
		}
		flow_t maxCapacity{};
		for (const ResidualVertex& vertex : graph ())
		{
			for (const ResidualEdge& edge : vertex)
			{
				if (capacities().capacity (edge.from ().index (), edge.to ().index ()) > maxCapacity)
				{
					maxCapacity = capacities ().capacity (edge.from ().index (), edge.to ().index ());
				}
			}
		}
		ResidualGraph& deltaGraph{ areDeltaEdgesRemoved () ? deltaGraphStorage : graph () };
		deltaGraph.setMatrix (true);
		LabelingSolver* pSubSolver;
		switch (subSolver())
		{
			case ESubSolver::FordFulkerson:
				pSubSolver = new FordFulkersonSolver{ deltaGraph, deltaGraph[source ().index ()], deltaGraph[sink ().index ()], capacities() };
				break;
			case ESubSolver::ShortestPath:
				pSubSolver = new ShortestPathSolver{ deltaGraph, deltaGraph[source ().index ()], deltaGraph[sink ().index ()], capacities() };
				break;
			default:
				throw std::invalid_argument{ "unknown sub solver" };
		}
		pSubSolver->setRemoveZeroEdges (areZeroEdgesRemoved ());
		DeltaEdgeSelector edgeSelector;
		Pathfinder pathfinder{ deltaGraph, deltaGraph[source().index ()], deltaGraph[sink().index ()] };
		edgeSelector.delta = static_cast<flow_t>(std::pow (2, std::floor (std::log2 (maxCapacity))));
		while (edgeSelector.delta >= 1)
		{
			if (areDeltaEdgesRemoved ())
			{
				for (ResidualVertex& vertex : graph ())
				{
					ResidualVertex& deltaVertex{ deltaGraph[vertex.index ()] };
					deltaVertex.destroyAllOutEdges ();
					for (ResidualEdge& edge : vertex)
					{
						if (*edge >= edgeSelector.delta)
						{
							deltaVertex.addOutEdge (edge.to ().index (), *edge);
						}
					}
				}
			}
			pSubSolver->solveWithEdgeSelector (edgeSelector);
			if (areDeltaEdgesRemoved ())
			{
				for (ResidualVertex& deltaVertex : deltaGraph)
				{
					ResidualVertex& vertex{ graph()[deltaVertex.index ()] };
					for (ResidualEdge& deltaEdge : deltaVertex)
					{
						*edgeOrCreate (vertex, graph ()[deltaEdge.to ().index ()]) = *deltaEdge;
					}
				}
			}
			edgeSelector.delta /= 2;
		}
		delete pSubSolver;
	}

	bool CapacityScalingSolver::DeltaEdgeSelector::operator() (const ResidualEdge& _edge)
	{
		return *_edge >= delta;
	}

}