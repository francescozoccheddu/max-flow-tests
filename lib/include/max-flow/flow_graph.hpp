#ifndef INCLUDED_MAX_FLOW_FLOW_GRAPH
#define INCLUDED_MAX_FLOW_FLOW_GRAPH

#include <max-flow/graph/generic/graph.hpp>
#include <max-flow/edge_flow.hpp>

namespace MaxFlow
{

	// Types

	MF_GG_D_TT_FD using FlowGraph = Graph::Generic::Graph<TVertexData, EdgeFlow<TEdgeData>>;
	MF_GG_D_TT_FD using FlowGraphVertex = FlowGraph<TVertexData, TEdgeData>::Vertex;
	MF_GG_D_TT_FD using FlowGraphEdge = FlowGraph<TVertexData, TEdgeData>::Edge;

}

#endif