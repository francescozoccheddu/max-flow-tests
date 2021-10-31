#include <max-flow\graphs\algorithms\distance_labeler.hpp>

#include <algorithm>
#include <stdexcept>
#include <queue>


namespace MaxFlow::Graphs::Algorithms
{

	bool DistanceLabeler::Label::operator==(const Label& _other) const
	{
		return m_valid && _other.m_valid && m_distance == _other.m_distance;
	}

	bool DistanceLabeler::Label::operator!=(const Label& _other) const
	{
		return m_valid && _other.m_valid && m_distance != _other.m_distance;
	}

	bool DistanceLabeler::Label::operator<(const Label& _other) const
	{
		return m_valid && _other.m_valid && m_distance < _other.m_distance;
	}

	bool DistanceLabeler::Label::operator<=(const Label& _other) const
	{
		return m_valid && _other.m_valid && m_distance <= _other.m_distance;
	}

	bool DistanceLabeler::Label::operator>(const Label& _other) const
	{
		return m_valid && _other.m_valid && m_distance > _other.m_distance;
	}

	bool DistanceLabeler::Label::operator>=(const Label& _other) const
	{
		return m_valid && _other.m_valid && m_distance >= _other.m_distance;
	}

	DistanceLabeler::Label::Label() : m_distance{}, m_valid{ false }
	{}

	DistanceLabeler::Label::Label(size_t _distance) : m_distance{ _distance }, m_valid{ true }
	{}

	size_t DistanceLabeler::Label::distance() const {
		if (!m_valid)
		{
			throw std::logic_error{ "invalid label" };
		}
		return m_distance;
	}

	size_t DistanceLabeler::Label::operator*() const {
		return distance();
	}

	bool DistanceLabeler::Label::valid() const {
		return m_valid;
	}

	DistanceLabeler::DistanceLabeler(ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink)
		: m_graph{ _graph }, m_source{ _source }, m_sink{ _sink }, m_labels{}
	{
		ResidualGraph::ensureSameGraph(_graph, _source.graph(), _sink.graph());
	}

	void DistanceLabeler::reset()
	{
		m_labels.resize(m_graph.verticesCount());
		std::fill(m_labels.begin(), m_labels.end(), Label{});
	}

	void DistanceLabeler::calculate(EdgeSelector& _edgeSelector)
	{
		reset();
		std::queue<ResidualVertex*> queue{};
		ResidualGraph transposed;
		transposed.setMatrix(false);
		transposed.addVertices(m_graph.verticesCount());
		for (const ResidualVertex& vertex : m_graph)
		{
			ResidualVertex& transposedVertex{ transposed[vertex.index()] };
			for (const ResidualEdge& edge : vertex)
			{
				if (*edge)
				{
					transposed[edge.to().index()].addOutEdge(transposedVertex, *edge);
				}
			}
		}
		m_labels[m_sink.index()] = { 0 };
		queue.push(&transposed[m_sink.index()]);
		while (!queue.empty())
		{
			ResidualVertex& vertex{ *queue.front() };
			queue.pop();
			for (ResidualEdge& edge : vertex)
			{
				if (!m_labels[edge.to().index()].valid() && _edgeSelector(edge))
				{
					m_labels[edge.to().index()] = *m_labels[edge.from().index()] + 1;
					queue.push(&edge.to());
				}
			}
		}
	}

	DistanceLabeler::Label DistanceLabeler::operator[](const ResidualVertex& _vertex) const
	{
		ResidualGraph::ensureSameGraph(_vertex.graph(), m_graph);
		return m_labels[_vertex.index()];
	}

	void DistanceLabeler::setLabel(ResidualVertex& _vertex, Label _label)
	{
		ResidualGraph::ensureSameGraph(m_graph, _vertex.graph());
		m_labels[_vertex.index()] = _label;
	}

	void DistanceLabeler::setDistance(ResidualVertex& _vertex, size_t _distance)
	{
		setLabel(_vertex, _distance);
	}

	void DistanceLabeler::resetDistance(ResidualVertex& _vertex)
	{
		setLabel(_vertex, {});
	}

	bool DistanceLabeler::isAdmissible(const ResidualEdge& _edge) const
	{
		const Label from{ (*this)[_edge.from()] };
		const Label to{ (*this)[_edge.to()] };
		return *_edge && from.valid() && to.valid() && *from == *to + 1;
	}

}