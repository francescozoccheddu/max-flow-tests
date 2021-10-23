#include <max-flow/solvers/labeling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>
#include <limits>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using namespace MaxFlow::Graphs::Algorithms;

namespace MaxFlow::Solvers
{

	constexpr bool removeZeroEdgesOnAugment = false;

	void labeling MF_S_PL
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
		do
		{
			pathfinder.calculate ();
			if (pathfinder.isSinkLabeled ())
			{
				augmentMax (pathfinder.begin (), pathfinder.end (), removeZeroEdgesOnAugment);
			}
		}
		while (pathfinder.isSinkLabeled ());
	}

}