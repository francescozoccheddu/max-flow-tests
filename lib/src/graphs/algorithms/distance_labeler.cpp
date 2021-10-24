#include <max-flow\graphs\algorithms\distance_labeler.hpp>

#include <algorithm>
#include <queue>

namespace MaxFlow::Graphs::Algorithms
{

	DistanceLabeler::DistanceLabeler (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink)
		: m_graph{ _graph }, m_source{ _source }, m_sink{ _sink }, m_distances{}
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph (), _sink.graph ());
	}

	void DistanceLabeler::reset (size_t _distance)
	{
		m_distances.resize (m_graph.verticesCount ());
		std::fill (m_distances.begin (), m_distances.end (), 0);
	}

	void DistanceLabeler::calculate (EdgeSelector& _edgeSelector)
	{
		reset (0);
		std::queue<ResidualVertex*> queue{};
		queue.push (&m_sink);
		size_t distance{};
		while (!queue.empty ())
		{
			distance++;
			ResidualVertex& vertex{ *queue.front () };
			queue.pop ();
			for (ResidualEdge& edge : vertex)
			{
				if (!m_distances[edge.to ().index ()] && edge.to () != m_sink && _edgeSelector(edge))
				{
					m_distances[edge.to ().index ()] = distance;
					queue.push (&edge.to ());
				}
			}
		}
	}

	size_t DistanceLabeler::operator[](const ResidualVertex& _vertex) const
	{
		ResidualGraph::ensureSameGraph (_vertex.graph (), m_graph);
		return m_distances[_vertex.index ()];
	}

	void DistanceLabeler::setDistance (ResidualVertex& _vertex, size_t _distance)
	{
		ResidualGraph::ensureSameGraph (m_graph, _vertex.graph ());
		m_distances.resize (m_graph.verticesCount ());
		m_distances[_vertex.index ()] = _distance;
	}

	bool DistanceLabeler::isAdmissible (const ResidualEdge& _edge) const
	{
		return *_edge && (*this)[_edge.from ()] == (*this)[_edge.to ()] + 1;
	}


}