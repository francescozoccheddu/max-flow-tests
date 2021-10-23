#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_EDGE_SELECTOR
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_EDGE_SELECTOR

#include <max-flow/graphs/residual.hpp>

namespace MaxFlow::Graphs::Algorithms
{

	class EdgeSelector
	{

	public:

		static EdgeSelector all;

		virtual bool operator() (const ResidualEdge& _edge);

	};

}

#endif