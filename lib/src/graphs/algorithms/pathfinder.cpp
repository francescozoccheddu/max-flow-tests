#include <max-flow\graphs\algorithms\pathfinder.hpp>

#include <algorithm>
#include <queue>

namespace MaxFlow::Graphs::Algorithms
{

	Pathfinder::Pathfinder (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink)
		: m_graph{ _graph }, m_source{ _source }, m_sink{ _sink }, m_predecessors (_graph.verticesCount (), nullptr)
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph (), _sink.graph ());
	}

	void Pathfinder::reset ()
	{
		m_predecessors.resize (m_graph.verticesCount ());
		std::fill (m_predecessors.begin (), m_predecessors.end (), nullptr);
	}

	void Pathfinder::calculate (const EdgeSelector& _edgeSelector)
	{
		reset ();
		setPredecessor (m_source, m_source);
		std::queue<ResidualVertex*> queue{};
		queue.push (&m_source);
		while (!queue.empty () && !isSinkLabeled ())
		{
			ResidualVertex& vertex{ *queue.front () };
			queue.pop ();
			for (ResidualEdge& edge : vertex)
			{
				if (*edge && !isLabeled (edge.to ()) && _edgeSelector.shouldVisit (edge))
				{
					setPredecessor (edge.to (), vertex);
					queue.push (&edge.to ());
				}
			}
		}
	}

	const ResidualVertex& Pathfinder::operator[](const ResidualVertex& _vertex) const
	{
		return const_cast<Pathfinder&>(*this)[const_cast<ResidualVertex&>(_vertex)];
	}


	ResidualVertex& Pathfinder::operator[](ResidualVertex& _vertex)
	{
		ResidualGraph::ensureSameGraph (_vertex.graph (), m_graph);
		return *m_predecessors[_vertex.index ()];
	}

	void Pathfinder::resetPredecessor (ResidualVertex& _vertex)
	{
		ResidualGraph::ensureSameGraph (m_graph, _vertex.graph ());
		m_predecessors.resize (m_graph.verticesCount ());
		m_predecessors[_vertex.index ()] = nullptr;
	}

	void Pathfinder::setPredecessor (ResidualVertex& _vertex, ResidualVertex& _predecessor)
	{
		ResidualGraph::ensureSameGraph (m_graph, _vertex.graph (), _predecessor.graph ());
		m_predecessors.resize (m_graph.verticesCount ());
		m_predecessors[_vertex.index ()] = &_predecessor;
	}

	void Pathfinder::setPredecessor (ResidualEdge& _edge)
	{
		setPredecessor (_edge.to (), _edge.from ());
	}

	bool Pathfinder::isLabeled (const ResidualVertex& _vertex) const
	{
		ResidualGraph::ensureSameGraph (_vertex.graph (), m_graph);
		return m_predecessors[_vertex.index ()];
	}

	bool Pathfinder::isSinkLabeled () const
	{
		return isLabeled (m_sink);
	}

	bool Pathfinder::isSourceLabeled () const
	{
		return isLabeled (m_source);
	}

	Pathfinder::IteratorC Pathfinder::begin () const
	{
		return IteratorC{ *this, m_sink };
	}

	Pathfinder::IteratorC Pathfinder::cbegin () const
	{
		return IteratorC{ *this, m_sink };
	}

	Pathfinder::IteratorM Pathfinder::begin ()
	{
		return IteratorM{ *this, m_sink };
	}

	Pathfinder::IteratorC Pathfinder::end () const
	{
		return IteratorC{ *this, m_source };
	}

	Pathfinder::IteratorC Pathfinder::cend () const
	{
		return IteratorC{ *this, m_source };
	}

	Pathfinder::IteratorM Pathfinder::end ()
	{
		return IteratorM{ *this, m_source };
	}

	bool Pathfinder::EdgeSelector::shouldVisit (const ResidualEdge& _edge) const
	{
		return true;
	}

}