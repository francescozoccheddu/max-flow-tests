#ifndef INCLUDED_MAX_FLOW_SOLVE
#define INCLUDED_MAX_FLOW_SOLVE

#include <max-flow/graphs/flow.hpp>

namespace MaxFlow
{

#pragma region Declaration

	// Enums

	enum class ESolver
	{
		Labeling
	};

	constexpr ESolver defaultSolver{ ESolver::Labeling };

	// Functions

	template<typename TVertexData, typename TEdgeData>
	void solveInPlace (Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

	template<typename TVertexData, typename TEdgeData>
	Graphs::FlowGraph<TVertexData, TEdgeData> solve (const Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, const Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, const Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

#pragma endregion

#pragma region Implementation

	// TODO

#pragma endregion

}

#endif