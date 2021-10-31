#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_DISTANCE_LABELER
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_DISTANCE_LABELER

#include <max-flow/graphs/residual.hpp>
#include <max-flow/utils/reference_equatable.hpp>
#include <max-flow/graphs/algorithms/edge_selector.hpp>
#include <vector>
#include <compare>

namespace MaxFlow::Graphs::Algorithms
{

	class DistanceLabeler final : public Utils::ReferenceEquatable
	{

	public:

		class Label final {

		private:

			size_t m_distance{};
			bool m_valid{ false };

		public:

			Label();
			Label(size_t distance);

			size_t distance() const;
			bool valid() const;

			size_t operator*() const;

			bool operator ==(const Label& _other) const;
			bool operator !=(const Label& _other) const;
			bool operator <(const Label& _other) const;
			bool operator <=(const Label& _other) const;
			bool operator >(const Label& _other) const;
			bool operator >=(const Label& _other) const;

		};

	private:

		ResidualGraph& m_graph;
		ResidualVertex& m_source, & m_sink;
		std::vector<Label> m_labels;

	public:

		DistanceLabeler(ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink);

		void reset();

		void calculate(EdgeSelector& _edgeSelector = EdgeSelector::all);

		void setLabel(ResidualVertex& _vertex, Label _label);

		void setDistance(ResidualVertex& _vertex, size_t _distance);

		void resetDistance(ResidualVertex& _vertex);

		Label operator[](const ResidualVertex& _vertex) const;

		bool isAdmissible(const ResidualEdge& _edge) const;

	};

}

#endif