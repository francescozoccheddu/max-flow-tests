#include <max-flow/solvers/labeling/capacity_scaling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/solvers/labeling/ford_fulkerson.hpp>
#include <max-flow/solvers/labeling/shortest_path.hpp>
#include <cmath>
#include <stdexcept>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using namespace MaxFlow::Graphs::Algorithms;
using namespace MaxFlow::Graphs;

namespace MaxFlow::Solvers::Labeling
{

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
		if (areDeltaEdgesRemoved ())
		{
			deltaGraphStorage.setMatrix (true);
			deltaGraphStorage.addVertices (graph ().verticesCount ());
		}
		flow_t minCapacity{};
		for (const ResidualVertex& vertex : graph ())
		{
			for (const ResidualEdge& edge : vertex)
			{
				if (capacities ().capacity (edge.from ().index (), edge.to ().index ()) > minCapacity)
				{
					minCapacity = capacities ().capacity (edge.from ().index (), edge.to ().index ());
				}
			}
		}
		ResidualGraph& deltaGraph{ areDeltaEdgesRemoved () ? deltaGraphStorage : graph () };
		PropagateCallback callback{ graph() };
		deltaGraph.setMatrix (true);
		LabelingSolver* pSubSolver;
		switch (subSolver ())
		{
			case ESubSolver::FordFulkerson:
				pSubSolver = new FordFulkersonSolver{ deltaGraph, deltaGraph[source ().index ()], deltaGraph[sink ().index ()], capacities () };
				break;
			case ESubSolver::ShortestPath:
				pSubSolver = new ShortestPathSolver{ deltaGraph, deltaGraph[source ().index ()], deltaGraph[sink ().index ()], capacities () };
				break;
			default:
				throw std::invalid_argument{ "unknown sub solver" };
		}
		pSubSolver->setRemoveZeroEdges (areZeroEdgesRemoved ());
		if (areDeltaEdgesRemoved ())
		{
			pSubSolver->setCallback (callback);
		}
		DeltaEdgeSelector edgeSelector;
		edgeSelector.delta = static_cast<flow_t>(std::pow (2, std::floor (std::log2 (minCapacity))));
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
			pSubSolver->solve ();
			edgeSelector.delta /= 2;
		}
		delete pSubSolver;
	}

	void CapacityScalingSolver::PropagateCallback::onAugmentMax (const LabelingSolver& _solver)
	{
		const Pathfinder::IteratorC start{ _solver.pathfinder ().begin () }, end{ _solver.pathfinder ().end () };
		flow_t minCapacity{ Algorithms::minCapacity(start, end) };
		for (Pathfinder::IteratorC it{ start }; it != end; ++it)
		{
			ResidualEdge& edge{ m_graph[it->from().index()][it->to().index()] };
			augment (edge, minCapacity, _solver.areZeroEdgesRemoved());
		}
	}

	CapacityScalingSolver::PropagateCallback::PropagateCallback (Graphs::ResidualGraph& _graph) : m_graph{ _graph }
	{}

	bool CapacityScalingSolver::DeltaEdgeSelector::operator() (const ResidualEdge& _edge)
	{
		return *_edge >= delta;
	}

}