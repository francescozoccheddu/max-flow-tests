#include <max-flow/graphs/base/edge.hpp>

namespace MaxFlow::Graph::Base
{

#pragma region Construction

	Edge::Edge (Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		Graph::ensureSameGraph (_from.graph (), _to.graph ());
		if (_pPrevious)
		{
			Vertex::ensureSameVertex (_from, _pPrevious->from ());
		}
		if (_pNext)
		{
			Vertex::ensureSameVertex (_from, _pNext->from ());
		}
	}

#pragma endregion

#pragma region Vertex interface

	void Edge::detachFromList ()
	{
		if (previous ())
		{
			m_pPrevious->m_pNext = next ();
		}
		if (next ())
		{
			m_pNext->m_pPrevious = previous ();
		}
	}

	void Edge::attachToList ()
	{
		if (previous ())
		{
			m_pPrevious->m_pNext = this;
		}
		if (next ())
		{
			m_pNext->m_pPrevious = this;
		}
	}

	const Edge* Edge::previous () const
	{
		return m_pPrevious;
	}

	Edge* Edge::previous ()
	{
		return m_pPrevious;
	}

	const Edge* Edge::next () const
	{
		return m_pNext;
	}

	Edge* Edge::next ()
	{
		return m_pNext;
	}

#pragma endregion

#pragma region Getters

	const Vertex& Edge::from () const
	{
		return *m_pFrom;
	}

	Vertex& Edge::from ()
	{
		return *m_pFrom;
	}

	const Vertex& Edge::to () const
	{
		return *m_pTo;
	}

	Vertex& Edge::to ()
	{
		return *m_pTo;
	}

	bool Edge::hasAntiParallel () const
	{
		return to ().hasOutEdge (from ());
	}

	const Edge& Edge::antiParallel () const
	{
		return to ().outEdge (from ());
	}

	Edge& Edge::antiParallel ()
	{
		return to ().outEdge (from ());
	}

	const Edge* Edge::antiParallelIfExists () const
	{
		return to ().outEdgeIfExists (from ());
	}

	Edge* Edge::antiParallelIfExists ()
	{
		return to ().outEdgeIfExists (from ());
	}

#pragma endregion

#pragma region Destruction

	void Edge::destroy ()
	{
		from ().destroyEdge (*this);
	}

#pragma endregion

}