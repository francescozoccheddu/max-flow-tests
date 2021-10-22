#include <max-flow\graphs\algorithms\labeler.hpp>

#include <algorithm>

namespace MaxFlow::Graphs::Algorithms
{

	Labeler::Labeler (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink)
		: m_graph{ _graph }, m_source{ _source }, m_sink{ _sink }, m_predecessors (_graph.verticesCount (), nullptr), m_queue{}
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph ());
		ResidualGraph::ensureSameGraph (_graph, _sink.graph ());
	}

	void Labeler::label (const EdgeSelector& _edgeSelector)
	{
		m_predecessors.resize (m_graph.verticesCount ());
		std::fill (m_predecessors.begin (), m_predecessors.end (), nullptr);
		m_predecessors[m_source.index ()] = &m_source;
		m_queue = {};
		m_queue.push (&m_source);
		while (!m_queue.empty () && !m_predecessors[m_sink.index ()])
		{
			ResidualVertex& vertex{ *m_queue.front () };
			m_queue.pop ();
			for (ResidualEdge& edge : vertex)
			{
				if (*edge && !m_predecessors[edge.to ().index ()] && _edgeSelector.shouldVisit (edge))
				{
					m_predecessors[edge.to ().index ()] = &vertex;
					m_queue.push (&edge.to ());
				}
			}
		}
	}

	const ResidualVertex& Labeler::operator[](const ResidualVertex& _vertex) const
	{
		return const_cast<Labeler&>(*this)[const_cast<ResidualVertex&>(_vertex)];
	}


	ResidualVertex& Labeler::operator[](ResidualVertex& _vertex)
	{
		ResidualGraph::ensureSameGraph (_vertex.graph (), m_graph);
		return *m_predecessors[_vertex.index ()];
	}

	bool Labeler::hasPathToSink () const
	{
		return m_predecessors[m_sink.index ()];
	}

	Labeler::IteratorC Labeler::begin () const
	{
		return IteratorC{ *this, m_sink };
	}

	Labeler::IteratorC Labeler::cbegin () const
	{
		return IteratorC{ *this, m_sink };
	}

	Labeler::IteratorM Labeler::begin ()
	{
		return IteratorM{ *this, m_sink };
	}

	Labeler::IteratorC Labeler::end () const
	{
		return IteratorC{ *this, m_source };
	}

	Labeler::IteratorC Labeler::cend () const
	{
		return IteratorC{ *this, m_source };
	}

	Labeler::IteratorM Labeler::end ()
	{
		return IteratorM{ *this, m_source };
	}

	bool Labeler::EdgeSelector::shouldVisit (const ResidualEdge& _edge) const
	{
		return true;
	}

}