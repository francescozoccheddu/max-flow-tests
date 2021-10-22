#include <max-flow/solvers/labeling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/graphs/algorithms/labeler.hpp>
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
		ResidualGraph::ensureSameGraph (_graph, _source.graph ());
		ResidualGraph::ensureSameGraph (_graph, _sink.graph ());
		_graph.setMatrix (true);
		if (removeZeroEdgesOnAugment)
		{
			removeZeroEdges (_graph);
		}
		else
		{
			removeBiZeroEdges (_graph);
		}
		Labeler labeler{ _graph, _source, _sink };
		do
		{
			labeler.label ();
			if (labeler.hasPathToSink ())
			{
				augmentMax (labeler.begin (), labeler.end (), removeZeroEdgesOnAugment);
			}
		}
		while (labeler.hasPathToSink ());
	}

}