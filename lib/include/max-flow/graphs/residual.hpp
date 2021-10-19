#ifndef INCLUDED_MAX_FLOW_ALGORITHMS_RESIDUAL
#define INCLUDED_MAX_FLOW_ALGORITHMS_RESIDUAL

#include <max-flow/graphs/generic/graph.hpp>
#include <max-flow/graphs/flow.hpp>

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

		MF_GG_TT_F ResidualGraph create (const FlowGraph<TVertexData, TEdgeData>& _flowGraph);

		const UnderlyingGraph& graph () const;

		MF_GG_TT_F void toFlowGraph (FlowGraph<TVertexData, TEdgeData>& _flowGraph) const;

	};

#pragma endregion
	
#pragma region Implementation

	MF_GG_TT ResidualGraph ResidualGraph::create(const FlowGraph<TVD, TED>& _flowGraph)
	{
			
	}

	MF_GG_TT void ResidualGraph::toFlowGraph (FlowGraph<TVD, TED>& _flowGraph) const
	{
		
	}

#pragma endregion

}

#endif