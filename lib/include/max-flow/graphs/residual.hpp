#ifndef INCLUDED_MAX_FLOW_ALGORITHMS_RESIDUAL
#define INCLUDED_MAX_FLOW_ALGORITHMS_RESIDUAL

#include <max-flow/graphs/generic/graph.hpp>
#include <max-flow/graphs/flow.hpp>
#include <utility>
#include <stdexcept>

namespace MaxFlow::Graphs
{

	using std::size_t;

#pragma region Declaration

	using ResidualGraph = Generic::Graph<void, flow_t>;
	using ResidualVertex = ResidualGraph::Vertex;
	using ResidualEdge = ResidualGraph::Edge;

	MF_GG_TT_F static ResidualGraph createResidualGraph (const FlowGraph<TVertexData, TEdgeData>& _flowGraph);

	MF_GG_TT_F void updateFlowsFromResidualGraph (const ResidualGraph& _residualGraph, FlowGraph<TVertexData, TEdgeData>& _flowGraph);

#pragma endregion

#pragma region Implementation

	MF_GG_TT ResidualGraph createResidualGraph (const FlowGraph<TVD, TED>& _flowGraph)
	{
		ResidualGraph graph;
		graph.setMatrix (true);
		graph.addVertices (_flowGraph.verticesCount ());
		for (const FlowGraph<TVD, TED>::Vertex& originalVertex : _flowGraph)
		{
			ResidualVertex& vertex{ graph[originalVertex.index ()] };
			for (const FlowGraph<TVD, TED>::Edge& originalEdge : originalVertex)
			{
				ResidualEdge* pExistingEdge{ vertex.outEdgeIfExists (originalEdge.to ().index ()) };
				ResidualEdge& edge{ pExistingEdge ? *pExistingEdge : vertex.addOutEdge (originalEdge.to ().index ()) };
				ResidualEdge& reverseEdge{ pExistingEdge ? edge.antiParallel () : graph[originalEdge.to ().index ()].addOutEdge (vertex.index ()) };
				*edge += originalEdge->residualCapacity ();
				*reverseEdge += originalEdge->flow ();
			}
		}
		return graph;
	}

	MF_GG_TT void updateFlowsFromResidualGraph (const ResidualGraph& _residualGraph, FlowGraph<TVD, TED>& _flowGraph)
	{
		if (_flowGraph.verticesCount () != _residualGraph.verticesCount ())
		{
			throw std::logic_error{ "vertices count mismatch" };
		}
		for (FlowGraphVertex<TVD, TED>& originalVertex : _flowGraph)
		{
			const ResidualVertex& residualVertex{ _residualGraph[originalVertex.index ()] };
			for (FlowGraphEdge<TVD, TED>& originalEdge : originalVertex)
			{
				const ResidualEdge* pResidualEdge{ residualVertex.outEdgeIfExists (originalEdge.to ().index ()) };
				const flow_t r{ pResidualEdge ? **pResidualEdge : 0 };
				if (originalEdge->capacity () > r
					|| (originalEdge->capacity () == r && originalEdge.from ().index () < originalEdge.to ().index ()))
				{
					originalEdge->setFlow (originalEdge->capacity () - r);
				}
				else
				{
					originalEdge->setFlow (0);
				}
			}
		}
	}

#pragma endregion

}

#endif