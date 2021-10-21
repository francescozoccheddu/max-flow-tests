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

	class ResidualGraph final
	{

	public:

		using UnderlyingGraph = Generic::Graph<size_t, flow_t>;

	private:

		UnderlyingGraph m_graph;

		ResidualGraph (const UnderlyingGraph& _graph);
		ResidualGraph (UnderlyingGraph&& _graph);

	public:

		MF_GG_TT_F static ResidualGraph create (const FlowGraph<TVertexData, TEdgeData>& _flowGraph);

		const UnderlyingGraph& graph () const;

		MF_GG_TT_F void updateFlows (FlowGraph<TVertexData, TEdgeData>& _flowGraph) const;

	};

#pragma endregion

#pragma region Implementation

	MF_GG_TT ResidualGraph ResidualGraph::create (const FlowGraph<TVD, TED>& _flowGraph)
	{
		UnderlyingGraph graph;
		graph.setMatrix (true);
		graph.addVertices (_flowGraph.verticesCount ());
		for (const FlowGraph<TVD, TED>::Vertex& originalVertex : _flowGraph)
		{
			UnderlyingGraph::Vertex& vertex{ graph[originalVertex.index ()] };
			for (const FlowGraph<TVD, TED>::Edge& originalEdge : originalVertex)
			{
				UnderlyingGraph::Edge* pExistingEdge{ vertex.outEdgeIfExists (originalEdge.to ().index ()) };
				UnderlyingGraph::Edge& edge{ pExistingEdge ? *pExistingEdge : vertex.addOutEdge (originalEdge.to ().index ()) };
				UnderlyingGraph::Edge& reverseEdge{ pExistingEdge ? edge.antiParallel () : graph[originalEdge.to ().index ()].addOutEdge (vertex.index ()) };
				*edge += originalEdge->residualCapacity();
				*reverseEdge += originalEdge->flow ();
			}
		}
		return ResidualGraph{ std::move (graph) };
	}

	MF_GG_TT void ResidualGraph::updateFlows (FlowGraph<TVD, TED>& _flowGraph) const
	{
		if (_flowGraph.verticesCount () != graph ().verticesCount ())
		{
			throw std::logic_error{ "vertices count mismatch" };
		}
		for (FlowGraphVertex<TVD, TED>& originalVertex : _flowGraph)
		{
			const UnderlyingGraph::Vertex& vertex{ m_graph[originalVertex.index ()] };
			for (FlowGraphEdge<TVD, TED>& originalEdge : originalVertex)
			{
				const flow_t r{ *vertex[m_graph[originalEdge.to ().index ()]] };
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