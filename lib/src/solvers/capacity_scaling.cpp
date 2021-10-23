#include <max-flow/solvers/capacity_scaling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/graphs/algorithms/labeler.hpp>
#include <cmath>
#include <vector>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;
using namespace MaxFlow::Graphs::Algorithms;

namespace MaxFlow::Solvers
{

	constexpr bool removeZeroEdgesOnAugment = false;
	constexpr bool removeDeltaEdges = false;

	struct DeltaEdgeSelector : public Labeler::EdgeSelector
	{

		flow_t delta{};

		inline bool shouldVisit (const ResidualEdge& _edge) const override
		{
			return *_edge >= delta;
		}

	};

	void capacityScaling MF_S_PL
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
		ResidualGraph deltaGraphStorage;
		if (removeDeltaEdges)
		{
			deltaGraphStorage.setMatrix (true);
			deltaGraphStorage.addVertices (_graph.verticesCount ());
		}
		ResidualGraph& deltaGraph{ removeDeltaEdges ? deltaGraphStorage : _graph };
		deltaGraph.setMatrix (true);
		DeltaEdgeSelector edgeSelector;
		Labeler labeler{ deltaGraph, deltaGraph[_source.index ()], deltaGraph[_sink.index ()] };
		edgeSelector.delta = static_cast<flow_t>(std::pow (2, std::floor (std::log2 (_maxCapacity))));
		while (edgeSelector.delta >= 1)
		{
			if (removeDeltaEdges)
			{
				for (ResidualVertex& vertex : _graph)
				{
					ResidualVertex& deltaVertex{ deltaGraph[vertex.index ()] };
					deltaVertex.destroyAllOutEdges ();
					for (ResidualEdge& edge : vertex)
					{
						if (*edge >= edgeSelector.delta)
						{
							deltaVertex.addOutEdge (edge.to ().index (), *edge);
						}
					}
				}
			}
			labeler.label (edgeSelector);
			while (labeler.isSinkLabeled ())
			{
				augmentMax (labeler.begin (), labeler.end (), removeZeroEdgesOnAugment);
				if (removeDeltaEdges)
				{
					for (ResidualEdge& edge : labeler)
					{
						ResidualEdge& originalEdge{ _graph[edge.from ().index ()][edge.to ().index ()] };
						*originalEdge = *edge;
					}
				}
				labeler.label ();
			}
			edgeSelector.delta /= 2;
		}
	}

}