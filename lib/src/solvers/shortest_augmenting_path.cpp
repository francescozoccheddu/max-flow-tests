#include <max-flow/solvers/shortest_augmenting_path.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/graphs/algorithms/labeler.hpp>
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
	constexpr bool detectMinCut = true;

	void shortestAugmentingPath MF_S_PL
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
		std::vector<size_t> distances{ labeler.distances () };
		std::vector<size_t> distanceCounts (detectMinCut ? _graph.verticesCount () : 0, 0);
		if (detectMinCut)
		{
			for (size_t distance : distances)
			{
				distanceCounts[distance]++;
			}
		}
		labeler.setPredecessor (_source, _source);
		ResidualVertex* pCurrent{ &_source };
		while (distances[_source.index ()] < _graph.verticesCount ())
		{
			const size_t distance{ distances[pCurrent->index ()] };
			bool foundAdmissibleEdge{ false };
			for (ResidualEdge& edge : *pCurrent)
			{
				if (*edge && distances[edge.to ().index ()] == distance - 1)
				{
					labeler.setPredecessor (edge);
					pCurrent = &edge.to ();
					if (pCurrent == &_sink)
					{
						augmentMax (labeler.begin (), labeler.end (), removeZeroEdgesOnAugment);
						pCurrent = &_source;
					}
					foundAdmissibleEdge = true;
					break;
				}
			}
			if (!foundAdmissibleEdge)
			{
				size_t minDistance{ std::numeric_limits<size_t>::max () };
				bool hasOutEdges{};
				for (ResidualEdge& edge : *pCurrent)
				{
					if (*edge && distances[edge.to ().index ()] < minDistance)
					{
						hasOutEdges = true;
						minDistance = distances[edge.to ().index ()];
					}
				}
				if (!hasOutEdges)
				{
					break;
				}
				distances[pCurrent->index ()] = minDistance + 1;
				pCurrent = &labeler[*pCurrent];
				if (detectMinCut)
				{
					distanceCounts[distance]--;
					distanceCounts[minDistance + 1]++;
					if (!distanceCounts[distance])
					{
						break;
					}
				}
			}
		}
	}

}