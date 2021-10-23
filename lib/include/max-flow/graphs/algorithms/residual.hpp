#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_RESIDUAL
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_RESIDUAL

#include <max-flow/graphs/residual.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>

namespace MaxFlow::Graphs::Algorithms
{

	constexpr bool defaultRemoveZeroEdgeOnAugment = true;

	void augment (ResidualEdge& _edge, flow_t _amount, bool _removeZeroEdge= defaultRemoveZeroEdgeOnAugment);

	flow_t antiparallelCapacity (const ResidualEdge& _edge);

	ResidualEdge& antiparallelEdgeOrCreate (ResidualEdge& _edge);

	ResidualEdge& edgeOrCreate (ResidualVertex& _from , ResidualVertex& _to);

	void removeZeroEdges (ResidualGraph& _graph);

	void removeBiZeroEdges (ResidualGraph& _graph);

	void addZeroEdges (Graphs::ResidualGraph& _graph);

	void augmentMax (Pathfinder::IteratorM _start, Pathfinder::IteratorM _end, bool _removeZeroEdge = defaultRemoveZeroEdgeOnAugment);

}

#endif