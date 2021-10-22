#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_RESIDUAL
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_RESIDUAL

#include <max-flow/graphs/residual.hpp>
#include <max-flow/graphs/algorithms/labeler.hpp>

namespace MaxFlow::Graphs::Algorithms
{

	constexpr bool defaultRemoveZeroEdgeOnAugment = true;

	void augment (Graphs::ResidualEdge& _edge, Graphs::flow_t _amount, bool _removeZeroEdge= defaultRemoveZeroEdgeOnAugment);

	Graphs::flow_t antiparallelCapacity (const Graphs::ResidualEdge& _edge);

	Graphs::ResidualEdge& antiparallelEdgeOrCreate (Graphs::ResidualEdge& _edge);

	Graphs::ResidualEdge& edgeOrCreate (Graphs::ResidualVertex& _from , Graphs::ResidualVertex& _to);

	void removeZeroEdges (Graphs::ResidualGraph& _graph);

	void removeBiZeroEdges (Graphs::ResidualGraph& _graph);

	void addZeroEdges (Graphs::ResidualGraph& _graph);

	void augmentMax (Labeler::IteratorM _start, Labeler::IteratorM _end, bool _removeZeroEdge = defaultRemoveZeroEdgeOnAugment);

}

#endif