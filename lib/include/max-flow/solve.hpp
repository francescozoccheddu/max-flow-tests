#ifndef INCLUDED_MAX_FLOW_SOLVE
#define INCLUDED_MAX_FLOW_SOLVE

#include <max-flow/graphs/generic/macros.hpp>
#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>

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

	MF_GG_TT_F void solve (Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

	MF_GG_TT_F Graphs::FlowGraph<TVertexData, TEdgeData> solve (const Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, const Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, const Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

	void solve (Graphs::ResidualGraph& _residualGraph, Graphs::ResidualGraph::UnderlyingGraph::Vertex& _source, Graphs::ResidualGraph::UnderlyingGraph::Vertex& _sink, ESolver _solver = defaultSolver);

#pragma endregion

#pragma region Implementation

	MF_GG_TT void solve (Graphs::FlowGraph<TVD, TED>& _graph, Graphs::FlowGraphVertex<TVD, TED>& _source, Graphs::FlowGraphVertex<TVD, TED>& _sink, ESolver _solver)
	{
		Graphs::Base::Graph::ensureSameGraph (_source.graph (), _graph);
		Graphs::Base::Graph::ensureSameGraph (_sink.graph (), _graph);
		Graphs::ResidualGraph residualGraph{ Graphs::ResidualGraph::create (_graph) };
		using ResidualVertex = Graphs::ResidualGraph::UnderlyingGraph::Vertex;
		solve (residualGraph, const_cast<ResidualVertex&>(residualGraph.graph ()[_source.index ()]), const_cast<ResidualVertex&>(residualGraph.graph ()[_sink.index ()]), _solver);
		residualGraph.updateFlows (_graph);
	}

	MF_GG_TT Graphs::FlowGraph<TVD, TED> solve (const Graphs::FlowGraph<TVD, TED>& _graph, const Graphs::FlowGraphVertex<TVD, TED>& _source, const Graphs::FlowGraphVertex<TVD, TED>& _sink, ESolver _solver)
	{
		Graphs::Base::Graph::ensureSameGraph (_source.graph (), _graph);
		Graphs::Base::Graph::ensureSameGraph (_sink.graph (), _graph);
		Graphs::FlowGraph<TVD, TED> copyGraph{ _graph };
		solve (copyGraph, copyGraph[_source.index ()], copyGraph[_sink.index ()]);
		return copyGraph;
	}

#pragma endregion

}

#endif