#include <max-flow/solvers/labeling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <queue>
#include <limits>
#include <vector>
#include <algorithm>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using namespace MaxFlow::Graphs::Algorithms;

namespace MaxFlow::Solvers
{

	void labeling (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink)
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph ());
		ResidualGraph::ensureSameGraph (_graph, _sink.graph ());
		_graph.setMatrix (true);
		if (defaultRemoveZeroEdgeOnAugment)
		{
			removeZeroEdges (_graph);
		}
		std::queue<ResidualVertex*> list{};
		std::vector<ResidualVertex*> predecessors{ _graph.verticesCount (), nullptr };
		// label node t
		predecessors[_sink.index ()] = &_source;
		// while t is labeled
		while (predecessors[_sink.index ()])
		{
			// unlabel all nodes
			// set pred(j):=0 for each j in N
			std::fill (predecessors.begin (), predecessors.end (), nullptr);
			// label node s and set LIST:={s}
			predecessors[_source.index ()] = &_source;
			list = {};
			list.push (&_source);
			// while LIST is not empty and t is unlabeled
			while (!list.empty () && !predecessors[_sink.index ()])
			{
				// remove a node i from LIST
				ResidualVertex& vertex{ *list.front () };
				list.pop ();
				// for each arc (i,j) in the residual network emanating from node i
				for (ResidualEdge& edge : vertex)
				{
					// if node j is unlabeled
					if (*edge && !predecessors[edge.to ().index ()])
					{
						// set pred(j):=i, label node j, and add j to LIST
						predecessors[edge.to ().index ()] = &vertex;
						list.push (&edge.to ());
					}
				}
			}
			// if t is labeled then augment
			if (predecessors[_sink.index ()])
			{
				// minR:=min{r:(i,j) in P}
				ResidualVertex* pVertex{ &_sink };
				Graphs::flow_t minR{ std::numeric_limits<Graphs::flow_t>::max () };
				while (pVertex != &_source)
				{
					ResidualVertex& previous{ *predecessors[pVertex->index()] };
					ResidualEdge& edge{ previous[*pVertex] };
					if (*edge < minR)
					{
						minR = *edge;
					}
					pVertex = &previous;
				}
				// augment minR along P
				pVertex = &_sink;
				while (pVertex != &_source)
				{
					ResidualVertex& previous{ *predecessors[pVertex->index()] };
					ResidualEdge& edge{ previous[*pVertex] };
					augment (edge, minR);
					pVertex = &previous;
				}
			}
		}
	}

}