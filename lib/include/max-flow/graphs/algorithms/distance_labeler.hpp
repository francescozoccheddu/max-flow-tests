#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_DISTANCE_LABELER
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_DISTANCE_LABELER

#include <max-flow/graphs/residual.hpp>
#include <max-flow/utils/reference_equatable.hpp>
#include <max-flow/graphs/algorithms/edge_selector.hpp>
#include <vector>

namespace MaxFlow::Graphs::Algorithms
{


	class DistanceLabeler final : public Utils::ReferenceEquatable
	{

	private:

		ResidualGraph& m_graph;
		ResidualVertex& m_source, & m_sink;
		std::vector<size_t> m_distances;

	public:

		DistanceLabeler (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink);

		void reset (size_t _distance=0);

		void calculate (EdgeSelector& _edgeSelector = EdgeSelector::all);

		void setDistance (ResidualVertex& _vertex, size_t _distance);

		size_t operator[](const ResidualVertex& _vertex) const;

		bool isAdmissible (const ResidualEdge& _edge) const;

	};

}

#endif