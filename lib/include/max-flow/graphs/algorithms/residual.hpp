#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_RESIDUAL
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_RESIDUAL

#include <max-flow/graphs/residual.hpp>

namespace MaxFlow::Graphs::Algorithms
{

	using std::size_t;

	constexpr bool defaultRemoveZeroEdgeOnAugment = true;

	void augment (Graphs::ResidualEdge& _edge, Graphs::flow_t _amount, bool _removeZeroEdge= defaultRemoveZeroEdgeOnAugment);

	Graphs::flow_t antiparallelCapacity (const Graphs::ResidualEdge& _edge);

	Graphs::ResidualEdge& antiparallelEdge (Graphs::ResidualEdge& _edge);

	void removeZeroEdges (Graphs::ResidualGraph& _graph);

	void removeBiZeroEdges (Graphs::ResidualGraph& _graph);

	void addZeroEdges (Graphs::ResidualGraph& _graph);

}

#endif