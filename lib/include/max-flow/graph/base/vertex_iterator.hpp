#ifndef INCLUDED_MAX_FLOW_GRAPH_BASE_VERTEX_ITERATOR
#define INCLUDED_MAX_FLOW_GRAPH_BASE_VERTEX_ITERATOR

#include <iterator>
#include <type_traits>
#include <cstddef>
#include <max-flow/graph/base/vertex.hpp>

namespace MaxFlow::Graph::Base
{

	class Vertex;

	template<bool constant, bool reversed>
	class VertexIterator
	{

	public:

		// Tags

		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::conditional_t<constant, const Vertex, Vertex>;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		// Friends

		friend class Graph;

		// Attributes

		pointer* m_p;

		// Construction

		VertexIterator (pointer* _pCurrent);


	public:

		// Getters

		const Vertex& operator*() const;
		reference operator*();

		const Vertex* operator->() const;
		pointer operator->();

		const Vertex& operator[](difference_type _diff) const;
		reference operator[](difference_type _diff);

		// Iteration

		VertexIterator& operator++();
		VertexIterator operator++(int);

		VertexIterator& operator--();
		VertexIterator operator--(int);

		VertexIterator operator+(difference_type _diff) const;
		VertexIterator operator-(difference_type _diff) const;

		VertexIterator& operator+=(difference_type _diff);
		VertexIterator& operator-=(difference_type _diff);

		// Comparison

		auto operator<=>(const VertexIterator&) const = default;

	};

	using VertexIteratorFM = VertexIterator<false, false>;
	using VertexIteratorFC = VertexIterator<true, false>;
	using VertexIteratorRM = VertexIterator<false, true>;
	using VertexIteratorRC = VertexIterator<true, true>;

#pragma region Implementation

#pragma region Construction

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>::VertexIterator (pointer* _pCurrent) : m_p{ _pCurrent }
	{}

#pragma endregion

#pragma region Getters

	template<bool constant, bool reversed>
	inline const Vertex& VertexIterator<constant, reversed>::operator* () const
	{
		return **m_p;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>::reference VertexIterator<constant, reversed>::operator* ()
	{
		return **m_p;
	}

	template<bool constant, bool reversed>
	inline const Vertex* VertexIterator<constant, reversed>::operator-> () const
	{
		return *m_p;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>::pointer VertexIterator<constant, reversed>::operator-> ()
	{
		return *m_p;
	}

	template<bool constant, bool reversed>
	inline const Vertex& VertexIterator<constant, reversed>::operator[] (difference_type _diff) const
	{
		return const_cast<VertexIterator&>(*this)[_diff];
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>::reference VertexIterator<constant, reversed>::operator[] (difference_type _diff)
	{
		if constexpr (reversed)
		{
			return *m_p[-_diff];
		}
		else
		{
			return *m_p[_diff];
		}
	}

#pragma endregion

#pragma region Iteration

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>& VertexIterator<constant, reversed>::operator++ ()
	{
		return *this += 1;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>& VertexIterator<constant, reversed>::operator-- ()
	{
		return *this -= 1;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed> VertexIterator<constant, reversed>::operator++ (int)
	{
		VertexIterator last{ *this };
		++* this;
		return last;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed> VertexIterator<constant, reversed>::operator-- (int)
	{
		VertexIterator last{ *this };
		--* this;
		return last;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed> VertexIterator<constant, reversed>::operator+ (difference_type _diff) const
	{
		return VertexIterator{ *this } += _diff;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed> VertexIterator<constant, reversed>::operator- (difference_type _diff) const
	{
		return VertexIterator{ *this } -= _diff;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>& VertexIterator<constant, reversed>::operator+= (difference_type _diff)
	{
		if constexpr (reversed)
		{
			m_p -= _diff;
		}
		else
		{
			m_p += _diff;
		}
		return *this;
	}

	template<bool constant, bool reversed>
	inline VertexIterator<constant, reversed>& VertexIterator<constant, reversed>::operator-= (difference_type _diff)
	{
		return *this += -_diff;
	}

#pragma endregion

}

#endif