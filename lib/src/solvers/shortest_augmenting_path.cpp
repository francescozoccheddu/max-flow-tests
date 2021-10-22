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
		std::vector<size_t> distances (_graph.verticesCount (), 0);
		std::vector<size_t> numb (detectMinCut ? _graph.verticesCount () : 0, 0);
		{
			std::queue<ResidualVertex*> queue{};
			queue.push (&_sink);
			if (detectMinCut)
			{
				numb[0] = 1;
			}
			size_t distance{};
			while (!queue.empty ())
			{
				distance++;
				ResidualVertex& vertex{ *queue.front () };
				queue.pop ();
				for (ResidualEdge& edge : vertex)
				{
					if (!distances[edge.to ().index ()] && edge.to () != _sink)
					{
						if (detectMinCut)
						{
							numb[distance] ++;
						}
						distances[edge.to ().index ()] = distance;
						queue.push (&edge.to ());
					}
				}
			}
		}
		Labeler labeler{ _graph, _source, _sink };
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
					numb[distance]--;
					numb[minDistance + 1]++;
					if (!numb[distance])
					{
						break;
					}
				}
			}
		}
	}

}