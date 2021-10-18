#include <max-flow/graph/base/graph.hpp>

#include <vector>
#include <utility>
#include <stdexcept>

namespace MaxFlow::Graph::Base
{

#pragma region Utils

	void Graph::ensureSameGraph (const Graph& _a, const Graph& _b)
	{
		if (&_a != &_b)
		{
			throw std::logic_error{ "not the same graph" };
		}
	}

	void Graph::updateIndices (size_t _first)
	{
		for (size_t i{ _first }; i < verticesCount (); i++)
		{
			(*this)[i].setIndex (i);
		}
	}

	void Graph::ensureValidVertexIndex (size_t _index) const
	{
		if (_index < 0)
		{
			throw std::out_of_range{ "index < 0" };
		}
		if (_index > verticesCount ())
		{
			throw std::out_of_range{ "index > verticesCount" };
		}
	}

#pragma endregion

#pragma region Destruction

	void Graph::destroyVertex (Vertex& _vertex)
	{
		ensureSameGraph (_vertex.graph (), *this);
		_vertex.destroyAllOutEdges ();
		m_vertices.erase (m_vertices.begin () + _vertex.index ());
		updateIndices (_vertex.index ());
		delete& _vertex;
	}

	void Graph::destroyVertex (size_t _vertex)
	{
		destroyVertex ((*this)[_vertex]);
	}

	void Graph::destroyAllVertices ()
	{
		for (Vertex& vertex : *this)
		{
			vertex.destroyAllOutEdges ();
			delete& vertex;
		}
		m_vertices.clear ();
	}

	Graph::~Graph ()
	{
		destroyAllVertices ();
	}

#pragma endregion

#pragma region Vertex interface

	void Graph::edgeAdded ()
	{
		m_edgesCount++;
	}

	void Graph::edgeRemoved ()
	{
		m_edgesCount--;
	}

#pragma endregion

#pragma region Graph interface

	Vertex& Graph::allocateVertex (size_t _index)
	{
		return *new Vertex{ *this, _index };
	}

#pragma endregion

#pragma region Vertex insertion

	void Graph::addNewValidatedVertex (Vertex& _vertex)
	{
		for (Vertex& vertex : *this)
		{
			vertex.vertexAdded (_vertex);
		}
		m_vertices.insert (m_vertices.begin () + _vertex.index (), &_vertex);
		updateIndices (_vertex.index () + 1);
		_vertex.setMatrix (hasMatrix ());
	}

	Vertex& Graph::addVertex ()
	{
		Vertex& vertex{ allocateVertex (verticesCount ()) };
		addNewValidatedVertex (vertex);
		return vertex;
	}


	Vertex& Graph::addVertexBefore (Vertex& _next)
	{
		ensureSameGraph (_next.graph (), *this);
		return addVertexBefore (_next.index ());
	}

	Vertex& Graph::addVertexBefore (size_t _next)
	{
		ensureValidVertexIndex (_next);
		Vertex& vertex{ allocateVertex (_next) };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	void Graph::swapVertices (Vertex& _a, Vertex& _b)
	{
		ensureSameGraph (_a.graph (), *this);
		ensureSameGraph (_b.graph (), *this);
		{
			const size_t aIndex{ _a.index () };
			_a.setIndex (_b.index ());
			_b.setIndex (aIndex);
		}
		std::swap (m_vertices[_a.index ()], m_vertices[_b.index ()]);
		for (Vertex& vertex : *this)
		{
			vertex.vertexSwapped (_a, _b);
		}
	}

	void Graph::swapVertices (size_t _a, size_t _b)
	{
		swapVertices ((*this)[_a], (*this)[_b]);
	}

#pragma endregion

#pragma region Assignment

	Graph& Graph::operator=(const Graph& _clone)
	{
		destroyAllVertices ();
		shrinkToFit ();
		reserve (_clone.capacity ());
		setMatrix (_clone.hasMatrix ());
		for (const Vertex& vertex : _clone)
		{
			addVertex ();
		}
		for (const Vertex& vertex : _clone)
		{
			Vertex& thisVert{ (*this)[vertex.index ()] };
			for (const Edge& edge : vertex)
			{
				thisVert.addOutEdge ((*this)[edge.to ().index ()]);
			}
		}
		return *this;
	}

	Graph& Graph::operator=(Graph&& _moved)
	{
		destroyAllVertices ();
		m_vertices.swap (_moved.m_vertices);
		std::swap (m_edgesCount, _moved.m_edgesCount);
		std::swap (m_hasMatrix, _moved.m_hasMatrix);
		for (Vertex& vertex : *this)
		{
			vertex.setGraph (*this);
		}
		return *this;
	}

#pragma endregion

#pragma region Getters

	size_t Graph::edgesCount () const
	{
		return m_edgesCount;
	}

	size_t Graph::verticesCount () const
	{
		return m_vertices.size ();
	}

	const Vertex& Graph::vertex (size_t _index) const
	{
		return const_cast<Graph&>(*this).vertex (_index);
	}

	Vertex& Graph::vertex (size_t _index)
	{
		ensureValidVertexIndex (_index);
		return *m_vertices[_index];
	}

	const Vertex& Graph::operator[] (size_t _index) const
	{
		return vertex (_index);
	}

	Vertex& Graph::operator[] (size_t _index)
	{
		return vertex (_index);
	}

#pragma endregion

#pragma region Iteration

	const Vertex** Graph::firstVertex () const
	{
		return const_cast<const Vertex**>(const_cast<Graph*>(this)->firstVertex ());
	}

	const Vertex** Graph::lastVertex () const
	{
		return const_cast<const Vertex**>(const_cast<Graph*>(this)->lastVertex ());
	}

	Vertex** Graph::firstVertex ()
	{
		return verticesCount () ? &m_vertices[0] : nullptr;
	}

	Vertex** Graph::lastVertex ()
	{
		return verticesCount () ? &m_vertices[0] + verticesCount () - 1 : nullptr;
	}

	const Vertex** Graph::increasePtr (const Vertex** _p, std::ptrdiff_t _diff)
	{
		return _p ? _p + _diff : nullptr;
	}

	Vertex** Graph::increasePtr (Vertex** _p, std::ptrdiff_t _diff)
	{
		return _p ? _p + _diff : nullptr;
	}

	VertexIterator<true, false> Graph::begin () const
	{
		return VertexIterator<true, false>{firstVertex ()};
	}

	VertexIterator<false, false> Graph::begin ()
	{
		return VertexIterator<false, false>{firstVertex ()};
	}

	VertexIterator<true, false> Graph::cbegin () const
	{
		return VertexIterator<true, false>{firstVertex ()};
	}

	VertexIterator<true, true> Graph::rbegin () const
	{
		return VertexIterator<true, true>{lastVertex ()};
	}

	VertexIterator<false, true> Graph::rbegin ()
	{
		return VertexIterator<false, true>{lastVertex ()};
	}

	VertexIterator<true, true> Graph::crbegin () const
	{
		return VertexIterator<true, true>{lastVertex ()};
	}

	VertexIterator<true, false> Graph::end () const
	{
		return VertexIterator<true, false>{increasePtr (lastVertex (), 1)};
	}

	VertexIterator<false, false> Graph::end ()
	{
		return VertexIterator<false, false>{increasePtr (lastVertex (), 1)};
	}

	VertexIterator<true, false> Graph::cend () const
	{
		return VertexIterator<true, false>{increasePtr (lastVertex (), 1)};
	}

	VertexIterator<true, true> Graph::rend () const
	{
		return VertexIterator<true, true>{increasePtr (firstVertex (), -1)};
	}

	VertexIterator<false, true> Graph::rend ()
	{
		return VertexIterator<false, true>{increasePtr (firstVertex (), -1)};
	}

	VertexIterator<true, true> Graph::crend () const
	{
		return VertexIterator<true, true>{increasePtr (firstVertex (), -1)};
	}

#pragma endregion

#pragma region Storage

	size_t Graph::capacity () const
	{
		return m_vertices.capacity ();
	}

	void Graph::reserve (size_t _capacity)
	{
		if (_capacity < 0)
		{
			throw std::out_of_range{ "capacity < 0" };
		}
		m_vertices.reserve (_capacity);
		for (Vertex& vertex : *this)
		{
			vertex.reserve (_capacity);
		}
	}

	void Graph::shrinkToFit ()
	{
		m_vertices.shrink_to_fit ();
		for (Vertex& vertex : *this)
		{
			vertex.shrinkToFit ();
		}
	}

	bool Graph::hasMatrix () const
	{
		return m_hasMatrix;
	}

	void Graph::setMatrix (bool _enabled)
	{
		if (_enabled != hasMatrix ())
		{
			m_hasMatrix = _enabled;
			for (Vertex& vertex : *this)
			{
				vertex.setMatrix (_enabled);
			}
		}
	}

#pragma endregion

}