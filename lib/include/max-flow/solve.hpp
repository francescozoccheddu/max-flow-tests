#ifndef INCLUDED_MAX_FLOW_SOLVE
#define INCLUDED_MAX_FLOW_SOLVE

#include <max-flow/edge_flow.hpp>

namespace MaxFlow
{

#pragma region Declaration

	// Types

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	using FlowGraph = Utils::Graph<TVertexData, EdgeFlow<TEdgeData>>;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	using FlowGraphVertex = FlowGraph<TVertexData, TEdgeData>::Vertex;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	using FlowGraphEdge = FlowGraph<TVertexData, TEdgeData>::Vertex;

	// Enums

	enum class ESolver
	{
		Labeling
	};

	constexpr ESolver defaultSolver{ ESolver::Labeling };

	// Functions

	template<typename TVertexData, typename TEdgeData>
	void solveInPlace (FlowGraph<TVertexData, TEdgeData>& _graph, FlowGraphVertex<TVertexData, TEdgeData>& _source, FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

	template<typename TVertexData, typename TEdgeData>
	FlowGraph<TVertexData, TEdgeData> solve (const FlowGraph<TVertexData, TEdgeData>& _graph, const FlowGraphVertex<TVertexData, TEdgeData>& _source, const FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

#pragma endregion

#pragma region Implementation

	// TODO

#pragma endregion

}

#endif