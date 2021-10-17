#ifndef INCLUDED_MAX_FLOW_GRAPH_BASE_EDGE_ITERATOR
#define INCLUDED_MAX_FLOW_GRAPH_BASE_EDGE_ITERATOR

#include <iterator>
#include <type_traits>
#include <cstddef>
#include <max-flow/graph/base/edge.hpp>

namespace MaxFlow::Graph::Base
{

	class Edge;

	template<bool constant, bool reversed>
	class EdgeIterator final
	{

	public:

		// Tags

		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::conditional_t<constant, const Edge, Edge>;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		// Friends

		friend class Vertex;

		// Attributes

		pointer m_p;

		// Construction

		EdgeIterator (pointer _pCurrent);

	public:

		// Getters

		const Edge& operator*() const;
		reference operator*();

		const Edge* operator->() const;
		pointer operator->();

		// Iteration

		EdgeIterator& operator++();
		EdgeIterator operator++(int);

		EdgeIterator& operator--();
		EdgeIterator operator--(int);

		// Comparison

		bool operator== (const EdgeIterator&) const = default;

	};

	using EdgeIteratorFM = EdgeIterator<false, false>;
	using EdgeIteratorFC = EdgeIterator<true, false>;
	using EdgeIteratorRM = EdgeIterator<false, true>;
	using EdgeIteratorRC = EdgeIterator<true, true>;

#pragma region Implementation

#pragma region Construction

	template<bool constant, bool reversed>
	inline EdgeIterator<constant, reversed>::EdgeIterator (pointer _pCurrent) : m_p{ _pCurrent }
	{}

#pragma endregion

#pragma region Getters

	template<bool constant, bool reversed>
	inline const Edge& EdgeIterator<constant, reversed>::operator* () const
	{
		return *m_p;
	}

	template<bool constant, bool reversed>
	inline EdgeIterator<constant, reversed>::reference EdgeIterator<constant, reversed>::operator* ()
	{
		return *m_p;
	}

	template<bool constant, bool reversed>
	inline const Edge* EdgeIterator<constant, reversed>::operator-> () const
	{
		return m_p;
	}

	template<bool constant, bool reversed>
	inline EdgeIterator<constant, reversed>::pointer EdgeIterator<constant, reversed>::operator-> ()
	{
		return m_p;
	}

#pragma endregion

#pragma region Iteration

	template<bool constant, bool reversed>
	inline EdgeIterator<constant, reversed>& EdgeIterator<constant, reversed>::operator++ ()
	{
		if constexpr (reversed)
		{
			m_p = m_p->previous ();
		}
		else
		{
			m_p = m_p->next ();
		}
		return *this;
	}

	template<bool constant, bool reversed>
	inline EdgeIterator<constant, reversed>& EdgeIterator<constant, reversed>::operator-- ()
	{
		if constexpr (reversed)
		{
			m_p = m_p->next ();
		}
		else
		{
			m_p = m_p->previous ();
		}
		return *this;
	}

	template<bool constant, bool reversed>
	inline EdgeIterator<constant, reversed> EdgeIterator<constant, reversed>::operator++ (int)
	{
		EdgeIterator last{ *this };
		++* this;
		return last;
	}

	template<bool constant, bool reversed>
	inline EdgeIterator<constant, reversed> EdgeIterator<constant, reversed>::operator-- (int)
	{
		EdgeIterator last{ *this };
		--* this;
		return last;
	}


#pragma endregion

#pragma endregion

}

#endif