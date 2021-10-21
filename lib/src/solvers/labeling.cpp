#include <max-flow/solvers/labeling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <queue>
#include <limits>

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
		if (_graph.verticesCount () >= std::numeric_limits<size_t>::max () - 1)
		{
			throw std::runtime_error{ "vertices count overflow" };
		}
		std::queue<ResidualVertex*> list{};
		// label node t
		*_sink = _source.index () + 1;
		// while t is labeled
		while (*_sink)
		{
			// unlabel all nodes
			// set pred(j):=0 for each j in N
			for (ResidualVertex& vertex : _graph)
			{
				*vertex = 0;
			}
			// label node s and set LIST:={s}
			*_source = _source.index () + 1;
			list = {};
			list.push (&_source);
			// while LIST is not empty and t is unlabeled
			while (!list.empty () && !*_sink)
			{
				// remove a node i from LIST
				ResidualVertex& vertex{ *list.front () };
				list.pop ();
				// for each arc (i,j) in the residual network emanating from node i
				for (ResidualEdge& edge : vertex)
				{
					// if node j is unlabeled
					if (*edge && !*edge.to ())
					{
						// set pred(j):=i, label node j, and add j to LIST
						*edge.to () = vertex.index () + 1;
						list.push (&edge.to ());
					}
				}
			}
			// if t is labeled then augment
			if (*_sink)
			{
				// minR:=min{r:(i,j) in P}
				ResidualVertex* pVertex{ &_sink };
				Graphs::flow_t minR{ std::numeric_limits<Graphs::flow_t>::max () };
				while (pVertex != &_source)
				{
					ResidualVertex& previous{ _graph[(**pVertex) - 1] };
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
					ResidualVertex& previous{ _graph[(**pVertex) - 1] };
					ResidualEdge& edge{ previous[*pVertex] };
					augment (edge, minR);
					pVertex = &previous;
				}
			}
		}
	}

}