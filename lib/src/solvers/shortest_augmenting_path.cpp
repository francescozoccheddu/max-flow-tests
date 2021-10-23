#include <max-flow/solvers/shortest_augmenting_path.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/graphs/algorithms/pathfinder.hpp>
#include <max-flow/graphs/algorithms/distance_labeler.hpp>
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
		ResidualGraph::ensureSameGraph (_graph, _source.graph (), _sink.graph ());
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
		DistanceLabeler distanceLabeler{ _graph, _source, _sink };
		std::vector<size_t> distanceCounts (detectMinCut ? _graph.verticesCount () : 0, 0);
		if (detectMinCut)
		{
			for (const ResidualVertex& vertex : _graph)
			{
				distanceCounts[distanceLabeler[vertex]]++;
			}
		}
		pathfinder.setPredecessor (_source, _source);
		ResidualVertex* pCurrent{ &_source };
		while (distanceLabeler[_source] < _graph.verticesCount ())
		{
			const size_t distance{ distanceLabeler[*pCurrent] };
			bool foundAdmissibleEdge{ false };
			for (ResidualEdge& edge : *pCurrent)
			{
				if (distanceLabeler.isAdmissible (edge))
				{
					pathfinder.setPredecessor (edge);
					pCurrent = &edge.to ();
					if (pCurrent == &_sink)
					{
						augmentMax (pathfinder.begin (), pathfinder.end (), removeZeroEdgesOnAugment);
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
					if (*edge && distanceLabeler[edge.to ()] < minDistance)
					{
						hasOutEdges = true;
						minDistance = distanceLabeler[edge.to ()];
					}
				}
				if (!hasOutEdges)
				{
					break;
				}
				distanceLabeler.setDistance (*pCurrent, minDistance + 1);
				pCurrent = &pathfinder[*pCurrent];
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