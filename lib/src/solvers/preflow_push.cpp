#include <max-flow/solvers/preflow_push.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>
#include <max-flow/graphs/algorithms/graphviz.hpp>
#include <vector>
#include <queue>
#include <limits>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using namespace MaxFlow::Graphs::Algorithms;

namespace MaxFlow::Solvers
{

	constexpr bool removeZeroEdgesOnAugment = false;

	void preflowPush MF_S_PL
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph (), _sink.graph());
		_graph.setMatrix (true);
		if (removeZeroEdgesOnAugment)
		{
			removeZeroEdges (_graph);
		}
		else
		{
			removeBiZeroEdges (_graph);
		}
		Pathfinder pathfinder{ _graph, _source, _sink };
		for (ResidualEdge& edge : _source)
		{
			augment (edge, *edge, removeZeroEdgesOnAugment);
		}

	}

}