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

		flow_t delta;

		inline bool shouldVisit (const ResidualEdge& _edge) const override
		{
			return *_edge >= delta;
		}

	};

	void capacityScalingWithEdgeSelector MF_S_PL
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
		DeltaEdgeSelector edgeSelector;
		edgeSelector.delta = static_cast<flow_t>(std::pow (2, std::log2 (_maxCapacity)));
		while (edgeSelector.delta >= 1)
		{
			labeler.label (edgeSelector);
			while (labeler.isSinkLabeled ())
			{
				augmentMax (labeler.begin (), labeler.end (), removeZeroEdgesOnAugment);
				labeler.label (edgeSelector);
			}
			edgeSelector.delta /= 2;
		}
	};

	void capacityScalingWithDeltaGraph MF_S_PL
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph ());
		ResidualGraph::ensureSameGraph (_graph, _sink.graph ());
		if (removeZeroEdgesOnAugment)
		{
			removeZeroEdges (_graph);
		}
		else
		{
			removeBiZeroEdges (_graph);
		}
		ResidualGraph deltaGraph;
		deltaGraph.addVertices (_graph.verticesCount ());
		deltaGraph.setMatrix (true);
		Labeler labeler{ deltaGraph, deltaGraph[_source.index ()], deltaGraph[_sink.index ()] };
		flow_t delta{ static_cast<flow_t>(std::pow (2, std::log2 (_maxCapacity))) };
		while (delta >= 1)
		{
			for (ResidualVertex& vertex : _graph)
			{
				ResidualVertex& deltaVertex{ deltaGraph[vertex.index ()] };
				for (ResidualEdge& edge : vertex)
				{
					if (*edge >= delta && !deltaVertex.hasOutEdge(edge.to().index()))
					{
						deltaVertex.addOutEdge (edge.to ().index (), *edge);
					}
				}
			}
			labeler.label ();
			while (labeler.isSinkLabeled ())
			{
				augmentMax (labeler.begin (), labeler.end (), removeZeroEdgesOnAugment);
				labeler.label ();
			}
			delta /= 2;
		}
		_graph = std::move (deltaGraph);
	};

	void capacityScaling MF_S_PL
	{
		auto func {removeDeltaEdges ? capacityScalingWithDeltaGraph : capacityScalingWithEdgeSelector};
		func (_graph, _source, _sink, _maxCapacity);
	};

}