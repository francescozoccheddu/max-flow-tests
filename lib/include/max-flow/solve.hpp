#ifndef INCLUDED_MAX_FLOW_SOLVE
#define INCLUDED_MAX_FLOW_SOLVE

#include <max-flow/graphs/generic/macros.hpp>
#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>

#define MF_S_PL (MaxFlow::Graphs::ResidualGraph& _graph, MaxFlow::Graphs::ResidualVertex& _source, MaxFlow::Graphs::ResidualVertex& _sink, MaxFlow::Graphs::flow_t _maxCapacity)

namespace MaxFlow
{

#pragma region Declaration

	// Enums

	enum class ESolver
	{
		Labeling, CapacityScaling, ShortestAugmentingPath, PreflowPush
	};

	constexpr ESolver defaultSolver{ ESolver::Labeling };

	// Functions

	MF_GG_TT_F void solve (Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

	MF_GG_TT_F Graphs::FlowGraph<TVertexData, TEdgeData>& solve (const Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, const  Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, const Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver);

	void solve (Graphs::ResidualGraph& _graph, Graphs::ResidualVertex& _source, Graphs::ResidualVertex& _sink, Graphs::flow_t _maxCapacity, ESolver _solver = defaultSolver);

	MF_GG_TT_F Graphs::flow_t maxCapacity (const Graphs::FlowGraph<TVertexData, TEdgeData>& _graph);

#pragma endregion

#pragma region Implementation

	MF_GG_TT void solve (Graphs::FlowGraph<TVD, TED>& _graph, Graphs::FlowGraphVertex<TVD, TED>& _source, Graphs::FlowGraphVertex<TVD, TED>& _sink, ESolver _solver)
	{
		Graphs::Base::Graph::ensureSameGraph (_source.graph (), _graph);
		Graphs::Base::Graph::ensureSameGraph (_sink.graph (), _graph);
		Graphs::ResidualGraph residualGraph{ Graphs::createResidualGraph (_graph) };
		residualGraph.setMatrix (true);
		solve (residualGraph, residualGraph[_source.index ()], residualGraph[_sink.index ()], maxCapacity(_graph), _solver);
		Graphs::updateFlowsFromResidualGraph (residualGraph, _graph);
	}

	MF_GG_TT Graphs::FlowGraph<TVD, TED> solve (const Graphs::FlowGraph<TVD, TED>& _graph, const Graphs::FlowGraphVertex<TVD, TED>& _source, const Graphs::FlowGraphVertex<TVD, TED>& _sink, ESolver _solver)
	{
		Graphs::Base::Graph::ensureSameGraph (_source.graph (), _graph);
		Graphs::Base::Graph::ensureSameGraph (_sink.graph (), _graph);
		Graphs::FlowGraph<TVD, TED> copyGraph{ _graph };
		solve (copyGraph, copyGraph[_source.index ()], copyGraph[_sink.index ()]);
		return copyGraph;
	}

	MF_GG_TT Graphs::flow_t maxCapacity (const Graphs::FlowGraph<TVD, TED>& _graph)
	{
		Graphs::flow_t capacity{};
		for (const Graphs::FlowGraphVertex<TVD, TED>& vertex : _graph)
		{
			for (const Graphs::FlowGraphEdge<TVD, TED>& edge : vertex)
			{
				if (edge->capacity () > capacity)
				{
					capacity = edge->capacity ();
				}
			}
		}
		return capacity;
	}

#pragma endregion

}

#endif