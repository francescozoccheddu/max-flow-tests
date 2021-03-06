#include <max-flow/graphs/base/vertex.hpp>

#include <vector>
#include <utility>
#include <stdexcept>

namespace MaxFlow::Graphs::Base
{

#pragma region Utils

	void Vertex::ensureSameVertex(const Vertex& _a, const Vertex& _b)
	{
		if (&_a != &_b)
		{
			throw std::logic_error{ "not the same vertex" };
		}
	}

#pragma endregion

#pragma region Construction

	Vertex::Vertex(Graph& _graph, size_t _index)
		: m_pGraph{ &_graph }, m_index{ _index }
	{
		_graph.ensureValidOrLastVertexIndex(_index);
	}

	Vertex::~Vertex()
	{
		setMatrix(false);
		while (m_pFirstOutEdge) {
			Edge* pEdge{ m_pFirstOutEdge };
			m_pFirstOutEdge = pEdge->m_pNext;
			delete pEdge;
		}
		m_pFirstOutEdge = nullptr;
	}

#pragma endregion

#pragma region Edge insertion

	void Vertex::addNewValidatedOutEdge(Edge& _edge)
	{
		_edge.attachToList();
		if (!_edge.next())
		{
			m_pLastOutEdge = &_edge;
		}
		if (!_edge.previous())
		{
			m_pFirstOutEdge = &_edge;
		}
		if (hasMatrix())
		{
			(*m_pOutVertexEdges)[_edge.to().index()] = &_edge;
		}
		m_outEdgesCount++;
		graph().edgeAdded();
	}

	void Vertex::ensureValidNewOutEdge(const Vertex& _to) const
	{
		Graph::ensureSameGraph(_to.graph(), graph());
		if (hasMatrix() && hasOutEdge(_to))
		{
			throw std::logic_error{ "edge already exists" };
		}
	}

	void Vertex::ensureValidNewOutEdgeBefore(const Vertex& _to, const Edge& _next) const
	{
		ensureValidNewOutEdge(_to);
		ensureSameVertex(_next.from(), *this);
	}

	Edge& Vertex::addOutEdge(Vertex& _to)
	{
		ensureValidNewOutEdge(_to);
		Edge& edge{ allocateEdge(_to, m_pLastOutEdge, nullptr) };
		addNewValidatedOutEdge(edge);
		return edge;
	}

	Edge& Vertex::addOutEdge(size_t _to)
	{
		return addOutEdge(graph()[_to]);
	}

	Edge& Vertex::addOutEdgeBefore(Vertex& _to, Edge& _next)
	{
		ensureValidNewOutEdgeBefore(_to, _next);
		Edge& edge{ allocateEdge(_to, _next.previous(), &_next) };
		addNewValidatedOutEdge(edge);
		return edge;
	}

	Edge& Vertex::addOutEdgeBefore(size_t _to, Edge& _next)
	{
		return addOutEdgeBefore(graph()[_to], _next);
	}

#pragma endregion

#pragma region Graph interface

	void Vertex::verticesAdded(size_t _index, size_t _count)
	{
		graph().ensureValidOrLastVertexIndex(_index);
		Graph::ensureValidCount(_count);
		if (hasMatrix())
		{
			m_pOutVertexEdges->insert(m_pOutVertexEdges->begin() + _index, _count, nullptr);
		}
	}

	void Vertex::verticesDestroyed(size_t _index, size_t _count)
	{
		graph().ensureValidVertexIndex(_index);
		Graph::ensureValidCount(_count);
		graph().ensureValidVertexIndex(_index + _count - 1);
		Edge* pEdge{ outEdgeIfExists(_index) };
		if (pEdge)
		{
			pEdge->destroy();
		}
		if (hasMatrix())
		{
			m_pOutVertexEdges->erase(m_pOutVertexEdges->begin() + _index, m_pOutVertexEdges->begin() + _index + _count);
		}
	}

	void Vertex::vertexSwapped(size_t _a, size_t _b)
	{
		graph().ensureValidVertexIndex(_a);
		graph().ensureValidVertexIndex(_b);
		if (hasMatrix())
		{
			std::swap((*m_pOutVertexEdges)[_a], (*m_pOutVertexEdges)[_a]);
		}
	}

	void Vertex::reserve(size_t _capacity)
	{
		if (hasMatrix())
		{
			m_pOutVertexEdges->reserve(_capacity);
		}
	}

	void Vertex::shrinkToFit()
	{
		if (hasMatrix())
		{
			m_pOutVertexEdges->shrink_to_fit();
		}
	}

	bool Vertex::hasMatrix() const
	{
		return m_pOutVertexEdges;
	}

	void Vertex::setMatrix(bool _enabled)
	{
		if (_enabled != hasMatrix())
		{
			if (_enabled)
			{
				m_pOutVertexEdges = new std::vector<Edge*>{};
				reserve(graph().capacity());
				m_pOutVertexEdges->insert(m_pOutVertexEdges->begin(), graph().verticesCount(), nullptr);
				for (Edge& edge : *this)
				{
					(*m_pOutVertexEdges)[edge.to().index()] = &edge;
				}
			}
			else
			{
				delete m_pOutVertexEdges;
				m_pOutVertexEdges = nullptr;
			}
		}
	}

	void Vertex::setIndex(size_t _index)
	{
		graph().ensureValidVertexIndex(_index);
		m_index = _index;
	}

	void Vertex::setGraph(Graph& _graph)
	{
		m_pGraph = &_graph;
	}

#pragma endregion

#pragma region Vertex interface

	Edge& Vertex::allocateEdge(Vertex& _to, Edge* _pPrevious, Edge* _pNext)
	{
		return *new Edge{ *this, _to, _pPrevious, _pNext };
	}

	Edge* Vertex::first()
	{
		return m_pFirstOutEdge;
	}

	Edge* Vertex::last()
	{
		return m_pLastOutEdge;
	}

#pragma endregion

#pragma region Getters

	const Graph& Vertex::graph() const
	{
		return *m_pGraph;
	}

	Graph& Vertex::graph()
	{
		return *m_pGraph;
	}

	size_t Vertex::index() const
	{
		return m_index;
	}

	size_t Vertex::outEdgesCount() const
	{
		return m_outEdgesCount;
	}

	bool Vertex::hasOutEdge(const Vertex& _to) const
	{
		Graph::ensureSameGraph(_to.graph(), graph());
		return outEdgeIfExists(_to.index());
	}

	bool Vertex::hasOutEdge(size_t _to) const
	{
		return outEdgeIfExists(_to);
	}

	const Edge& Vertex::outEdge(const Vertex& _to) const
	{
		Graph::ensureSameGraph(_to.graph(), graph());
		return outEdge(_to.index());
	}

	const Edge& Vertex::outEdge(size_t _to) const
	{
		return const_cast<Vertex&>(*this).outEdge(_to);
	}

	Edge& Vertex::outEdge(Vertex& _to)
	{
		Graph::ensureSameGraph(_to.graph(), graph());
		return outEdge(_to.index());
	}

	Edge& Vertex::outEdge(size_t _to)
	{
		Edge* pEdge{ outEdgeIfExists(_to) };
		if (!pEdge)
		{
			throw std::logic_error{ "no edge" };
		}
		return *pEdge;
	}

	const Edge* Vertex::outEdgeIfExists(const Vertex& _to) const
	{
		Graph::ensureSameGraph(_to.graph(), graph());
		return outEdgeIfExists(_to.index());
	}

	const Edge* Vertex::outEdgeIfExists(size_t _to) const
	{
		return const_cast<Vertex&>(*this).outEdgeIfExists(_to);
	}

	Edge* Vertex::outEdgeIfExists(Vertex& _to)
	{
		Graph::ensureSameGraph(_to.graph(), graph());
		return outEdgeIfExists(_to.index());
	}

	Edge* Vertex::outEdgeIfExists(size_t _to)
	{
		graph().ensureValidVertexIndex(_to);
		if (hasMatrix())
		{
			return (*m_pOutVertexEdges)[_to];
		}
		else
		{
			throw std::logic_error{ "no matrix" };
			/*
			for (Edge& edge : *this)
			{
				if (edge.to().index() == _to)
				{
					return &edge;
				}
			}
			return nullptr;
			*/
		}
	}

	const Edge& Vertex::operator[] (const Vertex& _to) const
	{
		return outEdge(_to);
	}

	const Edge& Vertex::operator[] (size_t _to) const
	{
		return outEdge(_to);
	}

	Edge& Vertex::operator[] (Vertex& _to)
	{
		return outEdge(_to);
	}

	Edge& Vertex::operator[] (size_t _to)
	{
		return outEdge(_to);
	}

#pragma endregion

#pragma region Iteration

	EdgeIterator<true, false> Vertex::begin() const
	{
		return EdgeIterator<true, false>{m_pFirstOutEdge};
	}

	EdgeIterator<false, false> Vertex::begin()
	{
		return EdgeIterator<false, false>{m_pFirstOutEdge};
	}

	EdgeIterator<true, false> Vertex::cbegin() const
	{
		return EdgeIterator<true, false>{m_pFirstOutEdge};
	}

	EdgeIterator<true, true> Vertex::rbegin() const
	{
		return EdgeIterator<true, true>{m_pLastOutEdge};
	}

	EdgeIterator<false, true> Vertex::rbegin()
	{
		return EdgeIterator<false, true>{m_pLastOutEdge};
	}

	EdgeIterator<true, true> Vertex::crbegin() const
	{
		return EdgeIterator<true, true>{m_pLastOutEdge};
	}

	EdgeIterator<true, false> Vertex::end() const
	{
		return EdgeIterator<true, false>{nullptr};
	}

	EdgeIterator<false, false> Vertex::end()
	{
		return EdgeIterator<false, false>{nullptr};
	}

	EdgeIterator<true, false> Vertex::cend() const
	{
		return EdgeIterator<true, false>{nullptr};
	}

	EdgeIterator<true, true> Vertex::rend() const
	{
		return EdgeIterator<true, true>{nullptr};
	}

	EdgeIterator<false, true> Vertex::rend()
	{
		return EdgeIterator<false, true>{nullptr};
	}

	EdgeIterator<true, true> Vertex::crend() const
	{
		return EdgeIterator<true, true>{nullptr};
	}

#pragma endregion

#pragma region Destruction

	void Vertex::destroyEdge(Edge& _edge)
	{
		ensureSameVertex(_edge.from(), *this);
		if (_edge == *m_pFirstOutEdge)
		{
			m_pFirstOutEdge = _edge.next();
		}
		if (_edge == *m_pLastOutEdge)
		{
			m_pLastOutEdge = _edge.previous();
		}
		if (hasMatrix())
		{
			(*m_pOutVertexEdges)[_edge.to().index()] = nullptr;
		}
		_edge.detachFromList();
		graph().edgeRemoved();
		m_outEdgesCount--;
		delete& _edge;
	}

	void Vertex::destroyEdge(size_t _to)
	{
		destroyEdge((*this)[_to]);
	}

	void Vertex::destroyAllOutEdges()
	{
		while (m_pFirstOutEdge)
		{
			destroyEdge(*m_pFirstOutEdge);
		}
	}

	void Vertex::destroy()
	{
		graph().destroyVertex(*this);
	}

#pragma endregion

}